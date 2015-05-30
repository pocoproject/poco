Biicode C/C++ dependency manager
=================================
New with biicode? Check the [Getting Started Guide](http://docs.biicode.com/c++/gettingstarted.html).

How to build it?
------------------
Building it is too easy:

    $ git clone git@github.com:pocoproject/poco.git
    $ cd poco
    $ bii init -l && bii work

It creates all the necessary structure to build POCO C++ with biicode. Then, you could run its default samples:

    $ bii build
    $ ./bin/any_executable

Run faster using several processors, e.g., using 4 cores:

    $ bii build -j4

**Note**: I recommend Windows OS users to use Visual Studio generator to build POCO project:

    $ bii configure -G "Visual Studio 10"
    $ bii build -j4
    $ ./bin/any_executable

By default, the first use applies all the changes to the repository, if you want to revert these ones, set the BII_POCO_REVERT_CHANGES environment variable to True and run bii work to keep your original code and undo the biicode changes.

    $ export BII_POCO_REVERT_CHANGES=True
    $ bii work


How to use it in other external projects?
-------------------------------------------

Take a look at any example from [examples/poco(develop) block](https://www.biicode.com/examples/examples/poco/develop) and try your own using *POCO C++ develop version* with biicode.
