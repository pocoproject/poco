POCO DNS-SD (Zeroconf) Wrapper Library for Bonjour and Avahi
============================================================

This is a POCO-based wrapper library providing an easy-to-use and
unified programming interface to Apple Bonjour and Avahi libraries
implementing DNS Service Discovery (DNS-SD, also known as Zeroconf).

Prerequisites
-------------

The [Apple Bonjour SDK](https://developer.apple.com/bonjour/) is needed on Windows (and OS X, of course). The Avahi client libraries are needed on Linux.

Getting Started
---------------

Clone into the root of an existing POCO source tree.


    $ git clone https://github.com/pocoproject/poco-dnssd.git DNSSD

On Linux, build with cmake like below.

    $ git apply DNSSD/CMakeLists.diff
    $ sudo ./build_cmake.sh

On Windows or OS X, build with cmake like below.

    $ git apply DNSSD/CMakeLists.diff
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make -j8
    
For build without using cmake on Windows, build the included Visual C++ solution. On Linux/OS X, build with POCO_BASE environment variable set to the root of
the POCO source tree.

    $ export POCO_BASE=`pwd`
    $ cd DNSSD
    $ make -s -j8
    $ make -s -j8 -C Default

See the [doc](https://github.com/pocoproject/poco-dnssd/tree/master/doc) directory for documentation and the samples directory for sample 
applications.


License
-------

Boost Software License 1.0
