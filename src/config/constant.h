/**
 * \file        constant.h
 * \author      Jamie Cui
 * \mainpage    BioSecure: Secure Bio-Statistics Computation Framework
 * \section     Prerequisite
 * \arg \c A **Linux distribution** of your choice (bioSecure was developed and tested with recent versions of [Debian](https://www.debian.org/) and [Ubuntu](http://www.ubuntu.com/)).
 * \arg \c **Required packages for bioSecure:**
     * *
        [`g++`](https://packages.debian.org/testing/g++) (version >=8)
     * * [`make`](https://packages.debian.org/testing/make)
     * * [`cmake`](https://packages.debian.org/testing/cmake)
     * * [`libgmp-dev`](https://packages.debian.org/testing/libgmp-dev)
     * * [`libssl-dev`](https://packages.debian.org/testing/libssl-dev)
     * * [`libboost-all-dev`](https://packages.debian.org/testing/libboost-all-dev) (version >= 1.66)

 * \arg **Required packages for REST:**
     * * [`libcpprest-dev`](https://github.com/Microsoft/cpprestsdk)

 * \section     Getting-Started
 *  First, do the following:
        \code
        $ cd biosecure
        $ mkdir build
        $ cd build
        \endcode
        Please make sure you have `g++(>=8)` set as default compiler, or you could compile using cmake flag `-DCMAKE_CXX_COMPILER=g++-8`.
        \code
        $ cmake .. -DCMAKE_CXX_COMPILER=g++-8
        \endcode
        Optionally, you may use `-DBUILD_TEST=On` to build tests.

        Then make the project.
        \code
        $ make
        \endcode
 *
 */

#ifndef BIOSECURE_CONSTANT_H
#define BIOSECURE_CONSTANT_H

#define DEBUG
#define MAX_FILE_LENGTH 500 //default = 500

#define MPC_PORT
#define SERVER_ADDR
#define CLIENT_ADDR

#endif //BIOSECURE_CONSTANT_H