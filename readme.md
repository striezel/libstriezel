# libthoro

This is the libthoro repository.
It contains code that is shared (or potentially shared) between multiple
projects by thoronador (hence the name). All code is in C++, some classes use
low-level C libraries, but encapsulate them. Let's shove the dirt under the
carpet, right where it belongs.

## Build status

[![Travis CI](https://travis-ci.org/Thoronador/libthoro.svg?branch=master)]
(https://travis-ci.org/Thoronador/libthoro)
[![drone.io](https://drone.io/github.com/Thoronador/libthoro/status.png)]
(https://drone.io/github.com/Thoronador/libthoro/latest)

## License

Most code is licensed under the GNU General Public License v3 as published by
the Free Software Foundation. A few individual files might use other
non-copyleft licenses, e.g. the MIT/X11 license. These files will have an
appropriate notice attached. However, any of those licenses should be
compatible with the GPL v3.

## Contents

Here is a short overview over the contents of the various directories. If you
want details about the classes, just look into the source code. Most of it is
documented well enough to understand it from the documentation alone.

* **archive/ar/** - classes to read Ar archives
* **archive/gzip/** - classes to uncompress GZIP files
* **archive/zip/** - classes to read ZIP archives
* **ci/** - contains script(s) for continous integration
* **cmake/** - files for the CMake build systems
* **common/** - code to do some "common" tasks
* **common/graphics/** - functions + classes to read various image formats
  (e.g. bitmap, JPEG, PNG, binary PPM) and prepare them for use as OpenGL
  textures
* **common/gui/** - incomplete GUI attempt based on GLUT
* **encoding/** - functions to convert strings between different encodings
* **filesystem/** - filesystem-related functions for directories and files
* **hash/** - classes that implement several hash algorithms from the "Secure
  Hash Algorithm" standard as described in FIPS PUB 180-1 and FIPS PUB 180-2;
  namely SHA-1, SHA-224, SHA-256, SHA-384 and SHA-512
* **iso9660/** - classes to read ISO 9660 disk images
* **os/** - class to detect the operating system (main focus on Linux)
* **procfs/** - functions that use the /proc file system from Linux
* **tests/** - test cases for various classes (but unfortunately not all
  classes are covered yet)
* **zlib/** - wrappers for compression and decompression with zlib
