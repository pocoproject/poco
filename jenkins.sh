#!/usr/bin/env bash

set -e
set -x

USE_CMAKE=
CONFIGURE_FLAGS=
CMAKE_FLAGS=
MAKE=make

case "$PLATFORM" in
    linux*)
        CONFIGURE_FLAGS="
          --cflags=-D_GLIBCXX_EXTERN_TEMPLATE=0
          --include-path=/opt/openssl/include
          --library-path=/opt/openssl/lib
        "
        ;;
    macosx)
        CONFIGURE_FLAGS="
          --include-path=/usr/local/opt/openssl/include
          --library-path=/usr/local/opt/openssl/lib
        "
        ;;
    solaris*)
        export PATH=/usr/sfw/bin:/usr/gnu/bin:/usr/bin:/usr/sbin:/sbin
        MAKE=gmake
        CONFIGURE_FLAGS="
          --config=SunOS-GCC
        "
        ;;
    freebsd)
        USE_CMAKE=true
        MAKE=gmake
        CMAKE_FLAGS=(
          -DCMAKE_CXX_FLAGS="-U_XOPEN_SOURCE -UPOCO_HAVE_FD_EPOLL"
          -DCMAKE_C_FLAGS="-U_XOPEN_SOURCE"
        )
        ;;
esac

if [[ -z $USE_CMAKE ]]; then
  ./configure --typical \
      --no-tests --no-samples \
      --static --shared \
      $CONFIGURE_FLAGS \
      --omit=PDF,Data,MongoDB,PageCompiler,CppParser
else
  [[ -d cmake_build ]] || mkdir cmake_build
  cd cmake_build
  cmake .. \
    "${CMAKE_FLAGS[@]}" \
    -DCMAKE_BUILD_TYPE=Release \
    $(for m in \
        PDF MONGODB DATA PAGECOMPILER CPPPARSER APACHECONNECTOR SEVENZIP REDIS POCODOC; do \
        echo -DENABLE_$m=OFF; done) \
    -DPOCO_STATIC=1
fi
$MAKE
