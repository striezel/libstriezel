/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011 thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "readJPEG.h"
#include <GL/gl.h>
#include <jpeglib.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>

/*
 * ERROR HANDLING:
 *
 * The JPEG library's standard error handler (jerror.c) is divided into
 * several "methods" which you can override individually.  This lets you
 * adjust the behavior without duplicating a lot of code, which you might
 * have to update with each future release.
 *
 * Our example here shows how to override the "error_exit" method so that
 * control is returned to the library's caller when a fatal error occurs,
 * rather than calling exit() as the standard error_exit method does.
 *
 * We use C's setjmp/longjmp facility to return control.  This means that the
 * routine which calls the JPEG library must first execute a setjmp() call to
 * establish the return point.  We want the replacement error_exit to do a
 * longjmp().  But we need to make the setjmp buffer accessible to the
 * error_exit routine.  To do this, we make a private extension of the
 * standard JPEG error handler object.  (If we were using C++, we'd say we
 * were making a subclass of the regular error handler.)
 *
 * Here's the extended error handler struct:
 */

struct my_error_mgr {
  struct jpeg_error_mgr pub; /* "public" fields */

  jmp_buf setjmp_buffer; /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */
METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


GLImageStructure readJPEG(const std::string& FileName)
{
  GLImageStructure result;
  result.setBuffer(NULL);
  result.setWidth(0);
  result.setHeight(0);
  result.setFormat(0);

  FILE * file_jpeg = fopen(FileName.c_str(), "rb");
  if (!file_jpeg)
  {
    std::cout << "File \""<<FileName<<"\" could not be opened for reading.\n";
    return result;
  }

  /* This struct contains the JPEG decompression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   */
  struct jpeg_decompress_struct cinfo;

  //struct jpeg_error_mgr jerr; // <--- standard error stuff
  struct my_error_mgr jerr;

  unsigned int row_stride;  /* physical row width in output buffer */

  /* Step 1: allocate and initialize JPEG decompression object */

  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(jerr.setjmp_buffer))
  {
    /* If we get here, the JPEG code has signaled an error.
     * We need to clean up the JPEG object, close the input file, and return.
     */
    jpeg_destroy_decompress(&cinfo);
    fclose(file_jpeg);
    //reset values
    result.setWidth(0);
    result.setHeight(0);
    result.setFormat(0);
    result.setBuffer(NULL);
    return result;
  }

  /* initialize the JPEG decompression object */
  jpeg_create_decompress(&cinfo);

  /* Step 2: specify data source (eg, a file) */
  jpeg_stdio_src(&cinfo, file_jpeg);

  /* Step 3: read file parameters with jpeg_read_header() */

  if (jpeg_read_header(&cinfo, TRUE)!=JPEG_HEADER_OK)
  {
  /* We can ignore the return value from jpeg_read_header since
   *   (a) suspension is not possible with the stdio data source, and
   *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
   * See libjpeg.doc for more info.
   */
    std::cout << "Error: jpeg_read_header returned with error.\n";
    return result;
  }
  /* Step 4: set parameters for decompression */

  /* In this example, we don't need to change any of the defaults set by
   * jpeg_read_header(), so we do nothing here.
   */
  cinfo.out_color_space = JCS_RGB;

  /* Step 5: Start decompressor */

  if( jpeg_start_decompress(&cinfo)!=TRUE)
  {
    /* We can ignore the return value since suspension is not possible
     * with the stdio data source.
     */
    std::cout << "Error: jpeg_start_decompress failed!\n";
    return result;
  }

  /* We may need to do some setup of our own at this point before reading
   * the data.  After jpeg_start_decompress() we have the correct scaled
   * output image dimensions available, as well as the output colormap,
   * if we asked for color quantization.
   * In this example, we need to make an output work buffer of the right size.
   */

  result.setWidth(cinfo.output_width);
  result.setHeight(cinfo.output_height);

  switch (cinfo.output_components)
  {
    case 3:
         result.setFormat(GL_RGB);
         break;
    case 4:
         result.setFormat(GL_RGBA);
         break;
    default:
         std::cout << "Colour type of file \""<<FileName<<"\" is neither RGB nor RGBA.\n";
         jpeg_destroy_decompress(&cinfo);
         fclose(file_jpeg);
         return result;
         break; //not really needed here...
  }//swi

  JSAMPARRAY buffer;		/* Output row buffer */

  /* JSAMPLEs per row in output buffer */
  row_stride = cinfo.output_width * cinfo.output_components;

  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)
      ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  char * v_ptr = (char*) malloc(row_stride*result.getHeight());
  const unsigned int row_size_in_bytes = row_stride;
  unsigned int offset = row_size_in_bytes*(result.getHeight()-1);

  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */

  /* Here we use the library's state variable cinfo.output_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   */
  while (cinfo.output_scanline < cinfo.output_height) {
    /* jpeg_read_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could ask for
     * more than one scanline at a time if that's more convenient.
     */
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    /* copy read line into our buffer
       We copy it the "wrong" way, i.e. we start at the last line and end with
       the first line, because OpenGL pixel data starts at bottom and goes up.
    */
    memcpy(&(v_ptr[offset]), buffer[0], row_size_in_bytes);
    offset -= row_size_in_bytes;
  }

  /* Step 7: Finish decompression */

  if (jpeg_finish_decompress(&cinfo)!=TRUE)
  {
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */
    std::cout << "Error: jpeg_finish_decompress failed!\n";
  }

  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);

  fclose(file_jpeg);

  /* At this point you may want to check to see whether any corrupt-data
   * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
   */

  /* And we're done! */

  result.setBuffer(v_ptr);
  v_ptr = NULL;
  return result;
}//function readJPEG

bool isJPEG(const std::string& FileName)
{
  FILE *file_jpeg = fopen(FileName.c_str(), "rb");
  if (!file_jpeg)
  {
    //file could not be opened for reading
    return false;
  }

  unsigned char header[2];
  memset(header, 0, 2);
  //read first two bytes
  if (fread(header, 1, 2, file_jpeg)!=2)
  {
    //file is not long enough to be a JPEG file
    fclose(file_jpeg);
    return false;
  }
  fclose(file_jpeg);
  return (//FF D8 FF E0 are the first bytes usually, but since some JPEGs put
          // the Exif before the JFIF header, we just check for the first two
          // bytes in the file. They should always be FF D8.
          (header[0]==255) and (header[1]==216));
}//function isJPEG
