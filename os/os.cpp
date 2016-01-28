/*
 -------------------------------------------------------------------------------
    This file is part of the Thoronador's common code library.
    Copyright (C) 2014, 2015, 2016  Thoronador

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

#include "os.hpp"
#include <iostream>
#if defined(__linux__) || defined(__linux)
  #include <sys/utsname.h> // for uname()
  #include <fstream>
  #include <vector>
  #include "../filesystem/file.hpp"
#endif // Linux

namespace libthoro
{

std::string os::detect()
{
  #if defined(_WIN64) || defined(__WIN64__)
  return "Windows (64bit)";
  #elif defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
  return "Windows (32bit)";
  #elif defined(__linux__) || defined(__linux)
  //some kind of Linux distribution
  return detectLinuxDistro();
  #else
    /* Feel free to extend that code for Mac OS or whatever you are using, and
       don't forget to send a pull request. */
    #error Unknown operating system! Please extend the function.
    //We should never get this far, error directive aborts compilation beforehand.
    return "unknown operating system";
  #endif // defined
}

#if defined(__linux__) || defined(__linux)
std::string checkForMatch(const std::string& str)
{
  std::vector<std::string> releases;
  releases.push_back("CentOS");
  releases.push_back("Debian GNU/Linux stretch/sid");
  releases.push_back("Debian GNU/Linux 8");
  releases.push_back("Debian GNU/Linux 7");
  releases.push_back("Debian GNU/Linux 6");
  releases.push_back("Debian");
  releases.push_back("Fedora");
  releases.push_back("openSUSE 13.2");
  releases.push_back("openSUSE 13.1");
  releases.push_back("openSUSE 12.3");
  releases.push_back("openSUSE 12.2");
  releases.push_back("openSUSE 12.1");
  releases.push_back("openSUSE 11.4");
  releases.push_back("openSUSE 11.3");
  releases.push_back("openSUSE 11.2");
  releases.push_back("openSUSE");
  releases.push_back("OpenSUSE");
  releases.push_back("SUSE Linux");
  releases.push_back("Ubuntu");

  std::vector<std::string>::const_iterator iter = releases.begin();
  while (iter != releases.end())
  {
    if (str.find(*iter) != std::string::npos)
      return *iter;
    ++iter;
  }//while
  return std::string("");
}

std::string getFirstLineInFile(const std::string& fileName)
{
  std::ifstream input;
  input.open(fileName.c_str(), std::ios::in | std::ios::binary);
  if (input.is_open() && input.good())
  {
    //file opened
    const unsigned int cMaxLine = 1024;
    char buffer[cMaxLine];
    std::string line = "";
    buffer[cMaxLine-1] = '\0';
    while (input.getline(buffer, cMaxLine-1))
    {
      line = std::string(buffer);
      if (!line.empty())
      {
        //found something
        input.close();
        return line;
      }
      buffer[cMaxLine-1] = '\0';
    }//while
    input.close();
  }//if open + good
  return std::string("");
}

std::string check_etc_files()
{
  std::string firstLine = "";
  //ArchLinux
  if (libthoro::filesystem::file::exists("/etc/arch-release"))
  {
    return "ArchLinux";
  }
  //CentOS
  if (libthoro::filesystem::file::exists("/etc/centos-release"))
  {
    firstLine = getFirstLineInFile("/etc/centos-release");
    if (!firstLine.empty())
      return firstLine;
    return "CentOS";
  }
  //Debian
  if (libthoro::filesystem::file::exists("/etc/debian_version"))
  {
    firstLine = getFirstLineInFile("/etc/debian_version");
    if (!firstLine.empty())
      return "Debian " + firstLine;
    return "Debian";
  }
  //Fedora
  if (libthoro::filesystem::file::exists("/etc/fedora-release"))
  {
    firstLine = getFirstLineInFile("/etc/fedora-release");
    if (!firstLine.empty())
      return firstLine;
    return "Fedora";
  }
  //Gentoo
  if (libthoro::filesystem::file::exists("/etc/gentoo-release"))
  {
    return "Gentoo";
  }
  //RedHat
  if (libthoro::filesystem::file::exists("/etc/redhat-release"))
  {
    firstLine = getFirstLineInFile("/etc/redhat-release");
    if (!firstLine.empty())
      return firstLine;
    return "RedHat";
  }
  //Slackware
  if (libthoro::filesystem::file::exists("/etc/slackware-version"))
  {
    return "Slackware";
  }
  //SuSE
  if (libthoro::filesystem::file::exists("/etc/SuSE-release"))
  {
    firstLine = getFirstLineInFile("/etc/SuSE-release");
    if (!firstLine.empty())
      return firstLine;
    return "SuSE";
  }

  //old generic code, just in case we did not get anywhere with the stuff above
  std::vector<std::string> files;
  files.push_back("/etc/issue");
  files.push_back("/etc/issue.net");
  files.push_back("/etc/lsb-base");
  files.push_back("/etc/release");
  files.push_back("/etc/version");

  std::vector<std::string>::const_iterator iter = files.begin();
  while (iter != files.end())
  {
    if (libthoro::filesystem::file::exists(*iter))
    {
      std::ifstream input;
      input.open(iter->c_str(), std::ios::in | std::ios::binary);
      if (input.is_open() && input.good())
      {
        //file opened
        const unsigned int cMaxLine = 1024;
        char buffer[cMaxLine];
        std::string line = "";
        buffer[cMaxLine-1] = '\0';
        while (input.getline(buffer, cMaxLine-1))
        {
          line = std::string(buffer);
          line = checkForMatch(line);
          if (!line.empty())
          {
            //found something
            input.close();
            return line;
          }
          buffer[cMaxLine-1] = '\0';
        }//while
        input.close();
      }//if open + good
    }//if file exists
    ++iter;
  }//while
  return std::string("");
}

std::string os::detectLinuxDistro()
{
  //check some files in /etc for distribution information
  std::string rel = check_etc_files();
  if (!rel.empty())
    return rel;

  //try output of uname() instead
  struct utsname data;
  int ret = uname(&data);
  if (ret>=0)
  {
    //uname() returned successfully
    rel = checkForMatch(data.sysname);
    if (!rel.empty()) return rel;
    rel = checkForMatch(data.nodename);
    if (!rel.empty()) return rel;
    rel = checkForMatch(data.release);
    if (!rel.empty()) return rel;
    rel = checkForMatch(data.version);
    if (!rel.empty()) return rel;
    rel = checkForMatch(data.machine);
    if (!rel.empty()) return rel;
  }

  //fallback
  return "Linux";
}
#endif //Linux

} //namespace libthoro
