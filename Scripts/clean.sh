#!/bin/sh
set -e
cd $(dirname $(readlink -f ${0}))
. ./config.sh

for PROJECT in $PROJECTS; do
  (cd $PROJECT && $MAKE clean)
done

rm -rf ../sysroot
rm -rf ../isodir
rm -rf ../fortuna.iso
