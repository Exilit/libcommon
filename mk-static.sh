#!/bin/bash

LIBSSL=providers/sources/openssl/libssl.a
LIBCRYPTO=providers/sources/openssl/libcrypto.a

function cleanup() {
	echo "Cleaning up tmp directory: " $TMPDIR
	rm -rf $TMPDIR
}

cd `dirname $0` || exit 1
ABSDIR=`pwd`
TMPDIR=`mktemp -d` || exit 1
echo "Preparing to merge object files in temporary directory..."
cp libcommon.a.stub $TMPDIR || exit 1
cp $LIBSSL $LIBCRYPTO $TMPDIR || exit 1
cd $TMPDIR || exit 1
echo "Extracting objects to temporary directory..."
ar x libcommon.a.stub || exit 1
ar x libssl.a || exit 1
ar x libcrypto.a || exit 1
echo "Creating new static library..."
echo ar rcs $ABSDIR/libcommon.a *.o
ar rcs $ABSDIR/libcommon.a *.o || exit 1
cd $ABSDIR
cleanup;
exit 0
