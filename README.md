wayland
=======

[![License](https://img.shields.io/badge/license-LGPLv3.0-blue.svg)](http://www.gnu.org/licenses/lgpl.txt)
[![GitHub release](https://img.shields.io/github/release/lirios/wayland.svg)](https://github.com/lirios/wayland)
[![Build Status](https://travis-ci.org/lirios/wayland.svg?branch=develop)](https://travis-ci.org/lirios/wayland)
[![GitHub issues](https://img.shields.io/github/issues/lirios/wayland.svg)](https://github.com/lirios/wayland/issues)

Wayland client and server extensions.

## Dependencies

Qt >= 5.12.0 with at least the following modules is required:

 * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
 * [qtdeclarative](http://code.qt.io/cgit/qt/qtdeclarative.git)
 * [qtwayland](http://code.qt.io/cgit/qt/qtwayland.git)

The following modules and their dependencies are required:

 * [cmake](https://gitlab.kitware.com/cmake/cmake) >= 3.10.0
 * [cmake-shared](https://github.com/lirios/cmake-shared.git) >= 1.0.0

## Installation

```sh
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/path/to/prefix ..
make
make install # use sudo if necessary
```

Replace `/path/to/prefix` to your installation prefix.
Default is `/usr/local`.

## Logging categories

Qt 5.2 introduced logging categories and we take advantage of
them to make debugging easier.

Please refer to the [Qt](http://doc.qt.io/qt-5/qloggingcategory.html) documentation
to learn how to enable them.

Available categories:

  * **liri.waylandclient:** Wayland client extensions
  * **liri.waylandserver:** Wayland server extensions

## Licensing

Licensed under the terms of the GNU Lesser General Public License version 3 or,
at your option, any later version.
