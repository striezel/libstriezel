#!/bin/bash

# Script to get some ZIP files for the test case in this directory and run the
# test afterwards.
#
#  Copyright (C) 2016  Dirk Stolle
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

# get directory of this script
THIS_DIR="${BASH_SOURCE%/*}"

# get first parameter
if [[ -z $1 ]]
then
  echo "First parameter must be executable file!"
  exit 1
fi
TEST_BINARY="$1"
# check, if it is a file
if [[ ! -f "$TEST_BINARY" ]]
then
  echo "Error: $TEST_BINARY is not a file or does not exist!"
  exit 1
fi
# check, if it is executable
if [[ ! -x "$TEST_BINARY" ]]
then
  echo "Error: $TEST_BINARY is not executable!"
  exit 1
fi

# get a zip file
wget https://github.com/madler/zlib/archive/v1.2.8.zip \
 --output-document="$THIS_DIR/zlib.zip"
if [[ $? -ne 0 ]]
then
  echo "Error: Could not download zlib's ZIP with wget!"
  exit 1
fi

# run the test
"$TEST_BINARY" "$THIS_DIR"
if [[ $? -ne 0 ]]
then
  rm -f "$THIS_DIR/zlib.zip"
  echo "Error: Test for ZIP entries failed!"
  exit 1
fi

#remove downloaded ZIP file
rm -f "$THIS_DIR/zlib.zip"

# everything is OK
exit 0
