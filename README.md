modec
=====

*modec* is a program to detect motion in video streams.

Purpose
-------

This software aims to aid in video surveillance by detecting human
motion.

Usage
-----

*modec* is started with an argument indicating video stream or file.

    $ ./modec /dev/video1

### key shortcuts

`F2` - video / detection blending window

`ESC` - close window (except main window)

Build dependencies
------------------

-   make

-   OpenCV v. 3.8.0

-   Qt v. 5

-   gtest v1.10.0 - for unit tests

-   lcov - for unit test coverage report

End note
--------

File `doc/readme.html` is automatically converted to `README.md` with
git commit using *pandoc* tool.
