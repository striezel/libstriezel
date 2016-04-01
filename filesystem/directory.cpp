/*
 -------------------------------------------------------------------------------
    This file is part of the Thoronador's common code library.
    Copyright (C) 2011, 2012, 2014, 2015, 2016  Thoronador

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
 -------------------------------------------------------------------------------
*/

#include "directory.hpp"
#include <sys/stat.h>

#if defined(_WIN32)
  #include <csdtio> //for std::remove()
  #include <cstring> //for std::memset()
  #include <Windows.h>
  #include <Shlobj.h>
#elif defined(__linux__) || defined(linux)
  #include <cstdlib> //for mkdtemp()
  #include <unistd.h>
  #include <pwd.h>
#endif

namespace libthoro
{

namespace filesystem
{

bool directory::exists(const std::string& dirName)
{
  struct stat buffer;
  if (stat(dirName.c_str(), &buffer)==0)
  {
    //stat() was successful
    return ((buffer.st_mode & S_IFMT) == S_IFDIR);
  }//if
  //An error occurred, so return false.
  return false;
}

bool directory::create(const std::string& dirName)
{
  #if defined(_WIN32)
    //WinAPI's CreateDirectory() returns nonzero on success
    return (CreateDirectory(dirName.c_str(), NULL)!=0);
  #elif defined(__linux__) || defined(linux)
    //mkdir() returns zero on success
    return (0==mkdir(dirName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH));
  #else
    #error Unknown operating system!
  #endif
}

bool directory::createRecursive(const std::string& dirName)
{
  const std::string::size_type delimPos = dirName.rfind(libthoro::filesystem::pathDelimiter);
  if (delimPos==std::string::npos) return create(dirName);

  if (exists(dirName.substr(0, delimPos)))
  {
    //parent directory already exists, just create the requested directory
    return create(dirName);
  }
  //recursive creation necessary, parent directory does not exist
  if (createRecursive(dirName.substr(0, delimPos)))
  {
    //creation of parent directory completed, go on with the final directory
    return create(dirName);
  }
  //creation of parent directory failed
  return false;
}

bool directory::createTemp(std::string& tempDirName)
{
  #if defined(_WIN32)
    /* Windows API has no function to create a temporary directory directly.
       Therefore we use an approach which involves several steps:
       - get the "basic" temporary path (e.g. "C:\Temp") with GetTempPath()
       - create a temporary file in that path with GetTempFileName()
       - delete aforementioned file and create directory with its name instead

       This approach contains a potential race condition: After the temporary
       file is deleted but before the directory is created, another application
       might create a file or directory with the same name. However, I do not
       know of any WinAPI functions that might mitigate or even avoid that race
       condition, so we leave it as it is for now.
    */
    char buffer[MAX_PATH+2];
    std::memset(buffer, '\0', MAX_PATH+2);
    const DWORD ret = GetTempPath(MAX_PATH+1, buffer);
    //function failed, return false
    if (ret == 0)
      return false;
    //buffer too small -> should not happen
    if (ret>MAX_PATH+1)
      return false;
    const std::string path = std::string(buffer);
    //fill buffer with zeros for next use
    memset(buffer, '\0', MAX_PATH+2);
    const UINT gtfn_ret = GetTempFileName(path.c_str(), "tmp", 0, buffer);
    if (gtfn_ret == 0)
      return false;
    //Path component too long?
    if (ERROR_BUFFER_OVERFLOW == gtfn_ret)
      return false;
    const tempFileName = std::string(buffer);
    //delete file ...
    if (std::remove(tempFileName.c_str()) != 0)
      return false;
    // ... and create directory with the same name
    if (createRecursive(tempFileName))
    {
      tempDirName = tempFileName:
      return true;
    }
    tempDirName.clear();
    return false;
  #elif defined(__linux__) || defined(linux)
    char tmpDir[] = "/tmp/directoryXXXXXXXXXX";
    const char * dirName = mkdtemp(tmpDir);
    if (nullptr == dirName) //null signals error
    {
      tempDirName.clear();
      return false;
    }
    tempDirName = std::string(dirName);
    return true;
  #else
    #error Unknown operating system!
  #endif
}

bool directory::remove(const std::string& dirName)
{
  #if defined(_WIN32)
    //WinAPI's RemoveDirectory() returns nonzero on success
    return (RemoveDirectory(dirName.c_str()) != 0);
  #elif defined(__linux__) || defined(linux)
    //rmdir() returns zero on success
    return (0 == rmdir(dirName.c_str()));
  #else
    #error Unknown operating system!
  #endif
}

bool directory::getHome(std::string& result)
{
  #if defined(_WIN32)
    char buffer[MAX_PATH+1];
    memset(buffer, 0, MAX_PATH+1);
    /*SHGetFolderLocation() is usually preferred over SHGetFolderPathA(), but
      the later one works on older Windows systems before Vista, too. */
    if (SHGetFolderPathA(0, CSIDL_PROFILE, NULL, 0, buffer)!=S_OK)
    {
      return false;
    }
    result = std::string(buffer);
    return true;
  #elif defined(__linux__) || defined(linux)
    const long int buf_size = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (buf_size <= -1)
      return false; //-1 means: sysconf() error / EINVAL
    char * buffer = new char[buf_size];
    struct passwd info;
    struct passwd * pwd_ptr = NULL;
    int error = getpwuid_r(getuid(), &info, buffer, buf_size, &pwd_ptr);
    if (error!=0)
    {
      //getpwuid_r() failed
      delete [] buffer;
      return false;
    }
    //success
    result = std::string(info.pw_dir);
    delete [] buffer;
    return true;
  #else
    #error Unknown operating system!
  #endif
}

std::string slashify(const std::string& path)
{
  if (path.empty()) return path;
  //Does it have a trailing (back)slash?
  if (path[path.length()-1]!=libthoro::filesystem::pathDelimiter)
  {
    return path + libthoro::filesystem::pathDelimiter;
  }
  return path;
}

std::string unslashify(const std::string& path)
{
  if (path.empty()) return path;
  if ((path[path.length()-1]==libthoro::filesystem::pathDelimiter) and (path.length()>1))
  {
    return path.substr(0, path.length()-1);
  }
  return path;
}

} //namespace filesystem

} //namespace libthoro
