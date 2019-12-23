# Secure-Two-party-K-means-
In this project,  we develope and test a Two-Party k-Means Clustering Protocol that guarantees privacy, and is more efficient than utilizing a general multiparty “compiler” to achieve the same task. 


## Requirements

* A **Linux distribution** of your choice (bioSecure (k-means) was developed and tested with recent versions of [Debian](https://www.debian.org/) and [Ubuntu](http://www.ubuntu.com/)).

* **Required packages for bioSecure:**
  * [`g++`](https://packages.debian.org/testing/g++) (version >=8)
    or another compiler and standard library implementing C++17 including the filesystem library
  * [`make`](https://packages.debian.org/testing/make)
  * [`cmake`](https://packages.debian.org/testing/cmake)
  * [`libgmp-dev`](https://packages.debian.org/testing/libgmp-dev)
  * [`libssl-dev`](https://packages.debian.org/testing/libssl-dev)
  * [`libboost-all-dev`](https://packages.debian.org/testing/libboost-all-dev) (version >= 1.66)
  
 * **Required packages for REST:**
     * [`libcpprest-dev`](https://github.com/Microsoft/cpprestsdk)

## Getting Started

First, clone this repository.

```
$ git clone https://github.com/jamie-cui/k-means.git
```
```
$ cd biosecure
$ mkdir build
$ cd build
```
Please make sure you have `g++(>=8)` set as default compiler, or you could compile using cmake flag `-DCMAKE_CXX_COMPILER=g++-8`.
```
$ cmake .. -DCMAKE_CXX_COMPILER=g++-8
```
Optionally, you may use `-DBUILD_TEST=On` to build tests.

Then make the project.
```
$ make
```
Also you can use [Doxygen](http://www.doxygen.nl/) to automatically generate docs
```
$ mkdir docs
$ doxygen