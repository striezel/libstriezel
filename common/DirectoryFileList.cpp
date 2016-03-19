/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011, 2015  Thoronador

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

#include "DirectoryFileList.hpp"
#include <iostream>
#include <fstream>

#if defined(_WIN32)
  //Windows includes go here
  #include <io.h>
#else
  //Linux directory entries
  #include <dirent.h>
#endif
#include <unistd.h>

FileEntry::FileEntry()
: FileName(""), IsDirectory(false)
{ }

FileEntry::FileEntry(const std::string& name, const bool isDir)
: FileName(name), IsDirectory(isDir)
{ }


bool getDirectoryFileList(const std::string& Directory, std::vector<FileEntry>& result, const std::string& Prefix, const bool recursive)
{
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
    std::cout << "getDirectoryFileList: ERROR: unable to open directory \""
              << Directory <<"\". Returning incomplete list.\n";
    return false;
  }
  //search it
  while(_findnext(handle, &sr)==0)
  {
    one.FileName = Prefix + std::string(sr.name);
    one.IsDirectory = ((sr.attrib & _A_SUBDIR)==_A_SUBDIR);
    result.push_back(one);
    if (recursive and one.IsDirectory
        and (std::string(sr.name)!=".") and (std::string(sr.name)!=".."))
    {
      //call function recursively
      if (!getDirectoryFileList(Directory+std::string(sr.name)+DirectorySeparator,
          result, Prefix+std::string(sr.name)+DirectorySeparator, recursive))
      {
        //error occurred, close handle and return
        _findclose(handle);
        return false;
      }
    }//if recursive
  }//while
  _findclose(handle);
  #else
  //Linux part
  DIR * direc = opendir(Directory.c_str());
  if (direc == NULL)
  {
    std::cout << "getDirectoryFileList: ERROR: unable to open directory \""
              << Directory <<"\". Returning incomplete list.\n";
    return false;
  }//if

  struct dirent* entry = readdir(direc);
  while (entry != NULL)
  {
    one.FileName = Prefix + std::string(entry->d_name);
    one.IsDirectory = entry->d_type==DT_DIR;
    //check for socket, pipes, block device and char device, which we don't want
    if (entry->d_type != DT_SOCK && entry->d_type != DT_FIFO && entry->d_type != DT_BLK
        && entry->d_type != DT_CHR)
    {
      result.push_back(one);
      if (recursive and one.IsDirectory
          and (std::string(entry->d_name)!=".") and (std::string(entry->d_name)!=".."))
      {
        //call function recursively
        if (!getDirectoryFileList(Directory+std::string(entry->d_name)+DirectorySeparator,
            result, Prefix+std::string(entry->d_name)+DirectorySeparator, recursive))
        {
          //error occurred, close handle and return
          closedir(direc);
          return false;
        }
      }//if recursive
    }//if wanted file type
    entry = readdir(direc);
  }//while
  closedir(direc);
  #endif
  return true;
}//function
