#!/usr/bin/env bash

set -Eeuxo pipefail

CONFIGURE_FLAGS=
CMAKE_FLAGS=()
MAKE=make
JOBS=2

case "$PLATFORM" in
  linux*)
    case "$PLATFORM" in
      linux32)
        TOOLCHAIN=/home/jenkins/toolchains/gcc-6.3-i686
        ;;
      linux64)
        TOOLCHAIN=/home/jenkins/toolchains/gcc-6.3-x86_64
        ;;
    esac
    export LD_LIBRARY_PATH=$TOOLCHAIN/lib
    if [[ -d $TOOLCHAIN/lib64 ]]; then
      LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TOOLCHAIN/lib64
    fi
    CMAKE_FLAGS+=(
      -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN/Toolchain.cmake
      -DOPENSSL_ROOT_DIR=/opt/openssl
    )
    JOBS=$(getconf _NPROCESSORS_ONLN)
    ;;
  macosx)
    CMAKE_FLAGS+=(
      -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl
    )
    JOBS=$(getconf _NPROCESSORS_ONLN)
    ;;
  freebsd*)
    MAKE=gmake
    CMAKE_FLAGS+=(
      -DCMAKE_CXX_FLAGS="-U_XOPEN_SOURCE -UPOCO_HAVE_FD_EPOLL"
      -DCMAKE_C_FLAGS="-U_XOPEN_SOURCE"
    )
    JOBS=$(sysctl -n hw.ncpu)
    ;;
esac

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
