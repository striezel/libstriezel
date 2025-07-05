:: Script to get an ISO9660 file for the corresponding test cases and run the
:: test afterwards.
::
::  Copyright (C) 2016, 2025  Dirk Stolle
::
::  This program is free software: you can redistribute it and/or modify
::  it under the terms of the GNU General Public License as published by
::  the Free Software Foundation, either version 3 of the License, or
::  (at your option) any later version.
::
::  This program is distributed in the hope that it will be useful,
::  but WITHOUT ANY WARRANTY; without even the implied warranty of
::  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
::  GNU General Public License for more details.
::
::  You should have received a copy of the GNU General Public License
::  along with this program.  If not, see <http://www.gnu.org/licenses/>.

@echo off

:: get directory of this script
SET THIS_DIR=%~dp0

:: get first parameter
if "%1" EQU "" (
  echo First parameter must be executable file path!
  exit /B 1
)
SET TEST_BINARY=%1
:: check, if it exists
if NOT EXIST "%TEST_BINARY%" (
  echo Error: %TEST_BINARY% does not exist!
  exit /B 1
)

if NOT EXIST "%THIS_DIR%\dsl-4.11.rc2.iso" (
  :: get the ISO image for Damn Small Linux
  wget http://distro.ibiblio.org/damnsmall/release_candidate/dsl-4.11.rc2.iso --output-document="%THIS_DIR%\dsl-4.11.rc2.iso"
  if %ERRORLEVEL% NEQ 0 (
    echo Error: Could not download DSL ISO  with wget!
    exit /B 1
  )
)

:: run the test
"%TEST_BINARY%" %THIS_DIR%
if %ERRORLEVEL% NEQ 0 (
  echo Error: Test for iso9660::archive class failed!
  exit /B 1
)

:: everything is OK
exit /B 0
