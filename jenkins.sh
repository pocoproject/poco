#!/usr/bin/env bash

set -Eeuxo pipefail
env


: "${TOOLCHAINS:=}"
: "${MUSL_GCC_VERSION:=15.1}"
JENKINS_PLATFORM=${PLATFORM}

git submodule update --init shared

. ./shared/build/ccache.sh

CONFIGURE_FLAGS=
CMAKE_FLAGS=(
  -DBUILD_SHARED_LIBS=off
  -DOPENSSL_ROOT_DIR=$(pwd)/openssl-${OPENSSL_VERSION}/usr
)
MAKE=make

echo "Testing platform $PLATFORM"
case "$PLATFORM" in
    linux*)
        case "$PLATFORM" in
            linux32)
                TOOLCHAIN=/home/jenkins/toolchains/gcc-6.3-multi
                ;;
            linux64)
                TOOLCHAIN=/home/jenkins/toolchains/gcc-6.3-x86_64
                ;;
            *)
                TOOLCHAIN=""
                ;;
        esac
        if [[ $TOOLCHAIN != "" ]]; then
            # Work around an optimizer bug on deb5.
            RELEASE_BUILD_TYPE=MinSizeRel
            export LD_LIBRARY_PATH=$TOOLCHAIN/lib
            if [[ -d $TOOLCHAIN/lib64 ]]; then
                LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TOOLCHAIN/lib64
            fi
            CMAKE_FLAGS+=(
                "-DCMAKE_C_FLAGS=-D_GLIBCXX_EXTERN_TEMPLATE=0"
                "-DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN/Toolchain.cmake"
            )
        fi
        ;;
    freebsd*)
        MAKE=gmake
        CMAKE_FLAGS+=(
            -DCMAKE_CXX_FLAGS="-U_XOPEN_SOURCE -UPOCO_HAVE_FD_EPOLL"
            -DCMAKE_C_FLAGS="-U_XOPEN_SOURCE"
        )
        ;;
    win*)
        CMAKE_FLAGS=(
            -DBUILD_SHARED_LIBS=off
            -DOPENSSL_ROOT_DIR=$(pwd)/openssl-${OPENSSL_VERSION}/OpenSSL
            -DPOCO_MT=ON
        )
        ;;
    mac*) 
        CMAKE_FLAGS+=(
            '-DCMAKE_OSX_ARCHITECTURES=x86_64;arm64'
            '-DCMAKE_OSX_DEPLOYMENT_TARGET=10.11'
        )
        ;;
esac

source ./shared/build/detect-platform.sh

if [ -z "${TOOLCHAINS}" ]; then
    TOOLCHAINS=/home/jenkins/toolchains
fi


PACKAGES=(
  CRYPTO
  ENCODINGS
  FOUNDATION
  JSON
  JWT
  NET
  NETSSL
  UTIL
  XML
  ZIP
)

PACKAGES_RE=$(echo "^(${PACKAGES[@]})\$" | perl -pe 's/ /|/g')

# Check for cross-compiler toolchain.
if [[ ${TOOLCHAIN_NAME} != none ]]; then
        JENKINS_PLATFORM="${TOOLCHAIN_NAME}"
        CMAKE_FLAGS+=(
            -DCMAKE_TOOLCHAIN_FILE=../shared/cmake/select-toolchain.cmake
            -DTOOLCHAINS=${TOOLCHAINS}
            -DMUSL_GCC_VERSION=${MUSL_GCC_VERSION}
            -DJENKINS_PLATFORM=${JENKINS_PLATFORM}
            -DOOKLA_DISABLE_THREAD_NAME=1 # Disable thread naming for musl builds
        )
fi

# Size optimization flags for non-MSVC builds
if [[ ${TOOLCHAIN_NAME} != none ]] || [[ ${PLATFORM} == linux* ]] || [[ ${PLATFORM} == mac* ]]; then
    CMAKE_FLAGS+=(
        "-DCMAKE_C_FLAGS_RELEASE=-O2 -DNDEBUG -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables"
        "-DCMAKE_CXX_FLAGS_RELEASE=-O2 -DNDEBUG -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables"
    )
fi

for build_type in Debug Release; do
  CMAKE_PACKAGES_FLAGS=()
  (
    build_dir="cmake_build_${build_type}"
    [[ -d ${build_dir} ]] || mkdir ${build_dir}
    cd ${build_dir}
    CMAKE_PACKAGES=$(cmake .. "${CMAKE_FLAGS[@]}" ${CMAKE_EXTRA} -Wno-dev -LA | grep '^ENABLE' | cut -c8- | sed -e 's,:BOOL=, ,'; true)
    while IFS='\n' read line; do
        read package enabled <<< "$line"
        echo "$package => $enabled"
        if [[ $enabled = ON ]] && [[ ! $package =~ $PACKAGES_RE ]]; then
          CMAKE_PACKAGES_FLAGS+=("-DENABLE_$package=OFF")
          echo "Disabling package $package"
        elif [[ $enabled = OFF ]] && [[ $package =~ $PACKAGES_RE ]]; then
          CMAKE_PACKAGES_FLAGS+=("-DENABLE_$package=ON")
          echo "Enabling package $package"
        else
          CMAKE_PACKAGES_FLAGS+=("-DENABLE_$package=$enabled")
        fi
    done <<< "${CMAKE_PACKAGES[@]}"

    cmake .. \
      "${CMAKE_FLAGS[@]}" \
       ${CMAKE_EXTRA} \
       -DCMAKE_TOOLCHAIN_FILE=../shared/cmake/select-toolchain.cmake \
       -DTOOLCHAINS=${TOOLCHAINS}  -DJENKINS_PLATFORM=${JENKINS_PLATFORM} \
      -DCMAKE_BUILD_TYPE=${build_type} \
      "${CMAKE_PACKAGES_FLAGS[@]}" \
      -DCMAKE_INSTALL_PREFIX="$(cd ..; pwd)/cmake_install_${build_type}"
   cmake --build . --config "${build_type}" --target install
  )
done

. ./shared/build/ccache.sh archive
