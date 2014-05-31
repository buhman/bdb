#!/bin/sh -xue

if type -p colorgcc > /dev/null ; then
   export CC=colorgcc
fi

autoreconf --force --install --verbose

./configure -C $@
