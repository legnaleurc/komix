#! /bin/sh

if [ $# -ne 1 ] || [ $1 != 'arm' -a $1 != 'x86' ] ; then
    echo 'arm or x86' >&2
    exit 1
fi

if [ -z "$ANDROID_NDK" ] ; then
    echo 'ANDROID_NDK is missing' >&2
    exit 1
fi

# if [ -z "$ANDROID_SDK" ] ; then
#     echo 'ANDROID_SDK is missing' >&2
#     exit 1
# fi

if [ -z "$ANDROID_QT5" ] ; then
    echo 'ANDROID_QT5 is missing' >&2
    exit 1
fi

PROJECT_ROOT=$(cd $(dirname $0); pwd)
ARCH=$1
ANDROID_BOOST="$PROJECT_ROOT/vendor/android-boost-$ARCH"
WORK_DIR="$PROJECT_ROOT/build-android-$ARCH"

if [ ! -d "$ANDROID_BOOST" ] ; then
    "$PROJECT_ROOT/vendor/build-boost.sh" $ARCH
fi

if [ ! -d $WORK_DIR ] ; then
    mkdir $WORK_DIR
fi

case "$ARCH" in
    arm )
        EXTRA_CMAKE_FLAGS="-DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.8 \
            -DANDROID_NDK_HOST_X64=1"
        ;;
    x86 )
        EXTRA_CMAKE_FLAGS="-DANDROID_TOOLCHAIN_NAME=x86-4.8 \
            -DANDROID_ABI=x86 \
            -DANDROID_NDK_HOST_X64=1"
        ;;
esac

cd "$WORK_DIR" >/dev/null

cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=../android.toolchain.cmake \
    -DANDROID_NATIVE_API_LEVEL=android-14 \
    -DANDROID_BOOST="$ANDROID_BOOST" \
    -DCMAKE_PREFIX_PATH="$ANDROID_QT5" \
    $EXTRA_CMAKE_FLAGS

make

cd - >/dev/null
