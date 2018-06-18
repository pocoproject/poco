#!/usr/bin/env bash

set -e
set -x

USE_CMAKE=
CONFIGURE_FLAGS=
CMAKE_FLAGS=()
MAKE=make
JOBS=2

case "$PLATFORM" in
  linux*)
    USE_CMAKE=true
    CONFIGURE_FLAGS="
      --cflags=-D_GLIBCXX_EXTERN_TEMPLATE=0
      --include-path=/opt/openssl/include
      --library-path=/opt/openssl/lib
    "
    CMAKE_FLAGS=(
      -DOPENSSL_ROOT_DIR=/opt/openssl
    )
    JOBS=$(getconf _NPROCESSORS_ONLN)
    ;;
  macosx)
    USE_CMAKE=true
    CONFIGURE_FLAGS="
      --include-path=/usr/local/opt/openssl/include
      --library-path=/usr/local/opt/openssl/lib
    "
    CMAKE_FLAGS=(
      -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl
    )
    JOBS=$(getconf _NPROCESSORS_ONLN)
    ;;
  solaris*)
    export PATH=/usr/sfw/bin:/usr/gnu/bin:/usr/bin:/usr/sbin:/sbin
    USE_CMAKE=true
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
    JOBS=$(sysctl -n hw.ncpu)
    ;;
esac

if [[ -z $USE_CMAKE ]]; then
  ./configure --typical \
      --no-tests --no-samples \
      --static --shared \
      $CONFIGURE_FLAGS \
      --omit=PDF,Data,MongoDB,PageCompiler,CppParser
  $MAKE -j${JOBS}
else
  for build_type in Debug Release; do
    (
      build_dir="cmake_build_${build_type}"
      [[ -d ${build_dir} ]] || mkdir ${build_dir}
      cd ${build_dir}
      cmake .. \
        "${CMAKE_FLAGS[@]}" \
        -DCMAKE_BUILD_TYPE=${build_type} \
        $(for m in \
            PDF MONGODB DATA PAGECOMPILER CPPPARSER APACHECONNECTOR SEVENZIP REDIS POCODOC; do \
            echo -DENABLE_$m=OFF; done) \
        -DPOCO_STATIC=1 \
        -DCMAKE_INSTALL_PREFIX="$(cd ..; pwd)/cmake_install_${build_type}"
      $MAKE -j${JOBS} all install
    )
  done
fi
