#!/bin/sh

PROJECT_TYPE="Unix Makefiles"
BUILD_TYPE="Debug"
BUILD_FLAGS="-m32"
SRC_DIR="~/work/gtest/googletest-1.10.x"
BUILD_DIR="build-x86"
PREFIX="~/lib/gtest"

printf "\nConfigure Google Test library...\n"
cmake \
    -G "${PROJECT_TYPE}" \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DCMAKE_CXX_FLAGS=${BUILD_FLAGS} \
    -S "${SRC_DIR}" \
    -B "${BUILD_DIR}" \
    -DCMAKE_INSTALL_PREFIX="${PREFIX}"
[ "$#" -ne "0" ] && {
    printf "Google Test configuration failed!\n"
    exit 1
}

printf "\nBuild Google Test library...\n"
cmake \
    --build "${BUILD_DIR}" \
    --config ${BUILD_TYPE}
[ "$#" -ne "0" ] && {
    printf "Google Test build failed!\n"
    exit 1
}

printf "\nInstall Google Test library...\n"
cmake \
    --install "${BUILD_DIR}" \
    --config ${BUILD_TYPE}
[ "$#" -ne "0" ] && {
    printf "Google Test installation failed!\n"
    exit 1
}

printf "\nGoogle Test library successfully installed!\n"
exit 0
