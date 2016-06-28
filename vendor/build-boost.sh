#! /bin/sh

VENDOR_ROOT="$(cd $(dirname $0); pwd)"
TESTED_BOOST_MAJOR_VERSION=1
TESTED_BOOST_MINOR_VERSION=55
TESTED_BOOST_PATCH_VERSION=0
TESTED_BOOST_VERSION=$TESTED_BOOST_MAJOR_VERSION.$TESTED_BOOST_MINOR_VERSION.$TESTED_BOOST_PATCH_VERSION
TESTED_BOOST=boost_"$TESTED_BOOST_MAJOR_VERSION"_"$TESTED_BOOST_MINOR_VERSION"_"$TESTED_BOOST_PATCH_VERSION"
BOOST_SOURCE_DIR="$VENDOR_ROOT/$TESTED_BOOST"

prepare () {
    . "$VENDOR_ROOT/vars.sh"

    PREFIX=$VENDOR_ROOT/android-boost-$ARCH
}

download () {
    if [ ! -d "$BOOST_SOURCE_DIR" ] ; then
        TMP=$PWD
        cd "$VENDOR_ROOT"

        curl -k -L -O "http://downloads.sourceforge.net/project/boost/boost/$TESTED_BOOST_VERSION/$TESTED_BOOST.tar.bz2"
        # TODO checksum
        tar -xjf "$TESTED_BOOST".tar.bz2

        cd $TMP
    fi
}

build () {
    variant=release
    target=android
    BUILD_DIR="$VENDOR_ROOT/build-boost-$ARCH"

    cd "$BOOST_SOURCE_DIR"

    if [ ! -e "b2" ] ; then
        # configure

        ./bootstrap.sh

        compileflags=
        for cflag in $EXTRA_CFLAGS ; do
            compileflags="$compileflags <compileflags>$cflag"
        done
        linkflags=
        for ldflag in $EXTRA_LDFLAGS ; do
            linkflags="$linkflags <linkflags>$ldflag"
        done
        cat > ./project-config.jam <<EOF
using gcc : android : $TOOLCHAIN/bin/$CROSS_PREFIX-g++ :
    <compileflags>--sysroot=$SYSROOT
    <compileflags>-O3
    <compileflags>-D__GLIBC__
    <compileflags>-D_GLIBCXX_HAVE_FENV_H=1
    $compileflags
    $linkflags
    <archiver>$TOOLCHAIN/bin/$CROSS_PREFIX-ar
    <ranlib>$TOOLCHAIN/bin/$CROSS_PREFIX-ranlib
;
EOF
    fi

    # build
    ./b2 --build-dir=$BUILD_DIR --prefix=$PREFIX --with-test toolset=gcc-android variant=$variant link=static threading=multi runtime-link=static install

    cd -
}

prepare "$1"
download
build
