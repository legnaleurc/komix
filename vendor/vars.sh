#! /bin/sh

# check ndk path
if [ -z $ANDROID_NDK ] ; then
    echo '$ANDROID_NDK is missing'
    exit 1
fi

GCC_VERSION='4.8'
PLATFORM_VERSION=14
HOST_OS=$(uname -s)
HOST_ARCH=$(uname -m)
ARCH="$1"

case "$HOST_ARCH" in
    i686 )
        HOST_ARCH="x86"
        ;;
    x86_64 )
        HOST_ARCH="x86_64"
        ;;
esac

case "$HOST_OS" in
    Linux )
        HOST_OS=linux
        ;;
    Darwin )
        HOST_OS=darwin
        ;;
    * )
        echo "unknown host OS: $HOST_OS"
        exit 1
        ;;
esac

case "$ARCH" in
    arm )
        TOOLCHAIN="arm-linux-androideabi-$GCC_VERSION"
        CROSS_PREFIX=arm-linux-androideabi
        ABI=armeabi-v7a
        ;;
    x86 )
        TOOLCHAIN="x86-$GCC_VERSION"
        CROSS_PREFIX=i686-linux-android
        ABI=x86
        ;;
    * )
        echo "unknown target arch: $ARCH"
        exit 1
        ;;
esac


# android ndk platform version
SYSROOT="$ANDROID_NDK/platforms/android-$PLATFORM_VERSION/arch-$ARCH"
TOOLCHAIN="$ANDROID_NDK/toolchains/$TOOLCHAIN/prebuilt/$HOST_OS-$HOST_ARCH"
STL_ROOT="$ANDROID_NDK/sources/cxx-stl/gnu-libstdc++/$GCC_VERSION"
EXTRA_CFLAGS="-I$STL_ROOT/include -I$STL_ROOT/libs/$ABI/include"
EXTRA_LDFLAGS="-L$STL_ROOT/libs/$ABI -lgnustl_static"
