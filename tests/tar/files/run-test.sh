#!/bin/bash

# Script to get a tar file for the corresponding test cases and run the
# test afterwards.
#
#  Copyright (C) 2016  Thoronador
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

if [[ ! -f "$THIS_DIR/grep-2.0.tar" ]]
then
  # check for .tar.gz file
  if [[ ! -f "$THIS_DIR/grep-2.0.tar.gz" ]]
  then
    # get the tar.gz archive for grep 2.0
    # That is a very old version, but we want small download size.
    wget http://ftp.gnu.org/gnu/grep/grep-2.0.tar.gz \
     --output-document="$THIS_DIR/grep-2.0.tar.gz"
    if [[ $? -ne 0 ]]
    then
      echo "Error: Could not download .tar.gz file with wget!"
      exit 1
    fi
  fi
  # extract .tar.gz
  zcat "$THIS_DIR/grep-2.0.tar.gz" > "$THIS_DIR/grep-2.0.tar"
  if [[ $? -ne 0 ]]
  then
    echo "Error: Could not extract $THIS_DIR/grep-2.0.tar.gz!"
    exit 1
  fi
fi

# run the test
"$TEST_BINARY" "$THIS_DIR"
if [[ $? -ne 0 ]]
then
  echo "Error: Test for tar::archive class failed!"
  exit 1
fi

# everything is OK
exit 0
