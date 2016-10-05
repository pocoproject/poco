POCO DNS-SD (Zeroconf) Wrapper Library for Bonjour and Avahi
============================================================

This is a POCO-based wrapper library providing an easy-to-use and
unified programming interface to Apple Bonjour and Avahi libraries
implementing DNS Service Discovery (DNS-SD, also known as Zeroconf).

Getting Started
---------------

1. Clone into the root of an existing POCO source tree. The POCO Foundation and Net 
libraries are required.

    $ git clone https://github.com/pocoproject/poco-dnssd.git DNSSD

2. On Windows, build the included Visual C++ solution. On Linux/OS X, build with POCO_BASE environment variable set to the root of
the POCO source tree.

    $ export POCO_BASE=`pwd`
    $ cd DNSSD
    $ make -s -j8

3. See the doc directory for documentation and the samples directory for sample 
applications.

License
-------

Boost Software License 1.0
