/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2015, 2016  Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "FileFunctions.hpp"
#include <sys/stat.h>
#include <utime.h>
#include <unistd.h>
#include <cmath>
#include "../common/StringUtils.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib> //for mkstemp()
#if defined(_WIN32)
  //Windows includes go here
  #include <io.h>
  #include <Windows.h>
#elif defined(__linux__) || defined(linux)
  //Linux directory entries
  #include <dirent.h>
#else
  #error "Unknown operating system!"
#endif

namespace libthoro
{

namespace filesystem
{

int64_t File::getSize64(const std::string& fileName)
{
  struct stat buffer;
  if (stat(fileName.c_str(), &buffer)==0)
  {
    //stat() was successful
    return buffer.st_size;
  }//if
  //An error occurred, so we don't have a proper value for file size.
  // Return -1 in this case to indicate an error.
  return -1;
}//function

bool File::setModificationTime(const std::string& FileName, const time_t new_mtime)
{
  struct stat buffer;
  if (stat(FileName.c_str(), &buffer)==0)
  {
    //stat() was successful
    if (buffer.st_mtime==new_mtime) return true;
    //change time
    struct utimbuf fileTimes;
    fileTimes.actime = buffer.st_atime;
    fileTimes.modtime = new_mtime;
    return (utime(FileName.c_str(), &fileTimes)==0);
  }//if
  //An error occurred, so return false.
  return false;
}

bool File::getSizeAndModificationTime(const std::string& FileName, int64_t& FileSize, time_t& FileTime)
{
  struct stat buffer;
  if (stat(FileName.c_str(), &buffer)==0)
  {
    //stat() was successful
    FileSize = buffer.st_size;
    FileTime = buffer.st_mtime;
    return true;
  }//if
  //An error occurred, so we don't have any proper values for that file.
  // Set values to -1 and return false in this case to indicate an error.
  FileSize = -1;
  FileTime = -1;
  return false;
}//function

bool File::exists(const std::string& FileName)
{
  return (access(FileName.c_str(), F_OK)==0);
}

bool File::remove(const std::string& fileName)
{
  return (std::remove(fileName.c_str())==0);
}

bool File::rename(const std::string& oldFileName, const std::string& newFileName)
{
  return (std::rename(oldFileName.c_str(), newFileName.c_str()) == 0);
}

bool File::createTemp(std::string& tempFileName)
{
  #if defined(_WIN32)
  //no windows implementation yet
  char buffer[MAX_PATH+2];
  memset(buffer, '\0', MAX_PATH+2);
  const DWORD ret = GetTempPath(MAX_PATH+1, buffer);
  //function failed, return false
  if (ret == 0)
    return false;
  //buffer too small -> should not happen
  if (ret>MAX_PATH+1)
    return false;
  std::string path = std::string(buffer);
  //fill buffer with zeros for next use
  memset(buffer, '\0', MAX_PATH+2);
  const UINT gtfn_ret = GetTempFileName(path.c_str(), "tmp", 0, buffer);
  if (gtfn_ret == 0)
    return false;
  //Path component too long?
  if (ERROR_BUFFER_OVERFLOW == gtfn_ret)
    return false;
  tempFileName = std::string(buffer);
  return true;
  #elif defined(__linux__) || defined(linux)
  char tmpFile[] = "/tmp/fileXXXXXXXXXX";
  const mode_t orig_umask = umask(S_IXUSR | S_IRWXG | S_IRWXO);
  const int fd = mkstemp(tmpFile);
  //reset umask
  umask(orig_umask);
  if (fd < 0) //-1 signals error
  {
    //Could not get temporary file name!
    unlink(tmpFile);
    return false;
  }
  close(fd);
  tempFileName = std::string(tmpFile);
  return true;
  #else
    //Unknown operating system.
    #error Unknown operating system!
    return false;
  #endif
}

bool File::readIntoString(const std::string& fileName, std::string& content)
{
  if (fileName.empty())
    return false;

  std::ifstream fileStream(fileName, std::ios_base::in | std::ios_base::binary);
  if (!fileStream.good())
  {
    std::cerr << "Error in File::readIntoString(): File could not be opened." << std::endl;
    return false;
  }
  std::string temp = "";
  content = "";
  while (!fileStream.eof() && std::getline(fileStream, temp, '\0'))
  {
    //append the delimiting NUL character, if necessary
    if (!content.empty())
      content.append(1, '\0');
    //append the data that was read from the file
    content.append(temp);
  }
  //File should be read until EOF, but failbit and badbit should not be set.
  if (!fileStream.eof() || fileStream.bad() || fileStream.fail())
  {
    fileStream.close();
    std::cerr << "Error in File::readIntoString(): "
              << "File could not be read." << std::endl;
    return false;
  }
  fileStream.close();
  return true;
}

float round(const float f)
{
  return floor(f + 0.5);
}

std::string getSizeString(const int64_t fileSize)
{
  if (fileSize<0) return "-"+getSizeString(-fileSize);

  //giga
  if (fileSize>1024*1024*1024)
  {
    return floatToString(round((fileSize*100.0f)/(1024.0f*1024*1024))/100.0f)+" GB";
  }
  if (fileSize>1024*1024)
  {
    return floatToString(round((fileSize*100.0f)/(1024.0f*1024))/100.0f)+" MB";
  }
  if (fileSize>1024)
  {
    return floatToString(round((fileSize*100.0f)/1024.0f)/100.0f)+" KB";
  }
  return floatToString(fileSize)+" byte";
}

std::vector<FileEntry> getDirectoryFileList(const std::string& Directory)
{
  std::vector<FileEntry> result;
  FileEntry one;
  #if defined(_WIN32)
  //Windows part
  intptr_t handle;
  struct _finddata_t sr;
  sr.attrib = _A_NORMAL | _A_RDONLY | _A_HIDDEN | _A_SYSTEM | _A_VOLID |
              _A_SUBDIR | _A_ARCH;
  handle = _findfirst(std::string(Directory+"*").c_str(),&sr);
  if (handle == -1)
  {
    std::cout << "getDirectoryFileList: ERROR: unable to open directory "
              <<"\""<<Directory<<"\". Returning empty list.\n";
    return result;
  }
  //search it
  while( _findnext(handle, &sr)==0)
  {
    one.fileName = std::string(sr.name);
    one.isDirectory = ((sr.attrib & _A_SUBDIR)==_A_SUBDIR);
    result.push_back(one);
  }//while
  _findclose(handle);
  #elif defined(__linux__) || defined(linux)
  //Linux part
  DIR * direc = opendir(Directory.c_str());
  if (direc == NULL)
  {
    std::cout << "Dusk::getDirectoryFileList: ERROR: unable to open directory "
              <<"\""<<Directory<<"\". Returning empty list.\n";
    return result;
  }//if

  struct dirent* entry = readdir(direc);
  while (entry != NULL)
  {
    one.fileName = std::string(entry->d_name);
    one.isDirectory = entry->d_type==DT_DIR;
    //check for socket, pipes, block device and char device, which we don't want
    if (entry->d_type != DT_SOCK && entry->d_type != DT_FIFO && entry->d_type != DT_BLK
        && entry->d_type != DT_CHR)
    {
      result.push_back(one);
    }
    entry = readdir(direc);
  }//while
  closedir(direc);
  #else
    #error "Unknown operating system!"
  #endif
  return result;
}//function

void splitPathFileExtension(const std::string& fileName, const char pathSeperator, std::string& path, std::string& name, std::string& extension)
{
  const std::string::size_type len = fileName.length();
  if (len==0)
  {
    path = "";
    name = "";
    extension = "";
    return;
  }

  //split path from file and ext.
  const std::string::size_type sepPos = fileName.rfind(pathSeperator);
  if (sepPos==std::string::npos)
  {
    path = "";
    name = fileName;
  }
  else
  {
    path = fileName.substr(0, sepPos+1);
    name = fileName.substr(sepPos+1);
  }
  // => now path has the path (including seperator), and name has the file including extension

  //split extension from name
  const std::string::size_type dotPos = fileName.rfind('.');
  if (dotPos==std::string::npos)
  {
    extension = "";
    return;
  }
  extension = name.substr(dotPos+1);
  name = name.substr(0, dotPos);
  return;
}

} //namespace filesystem

} //namespace libt
