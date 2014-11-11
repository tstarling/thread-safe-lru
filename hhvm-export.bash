#!/bin/bash

if [ -z "$1" ]; then
	echo "Usage: $0 <hhvm-dir>"
	exit 1
fi

dir=`dirname $0`
dest="$1/hphp/util"
sed -f hhvm.sed "$dir/thread-safe-lru/lru-cache.h" > "$dest/thread-safe-lru-cache.h"
sed -f hhvm.sed "$dir/thread-safe-lru/scalable-cache.h" > "$dest/thread-safe-scalable-cache.h"
sed -f hhvm.sed "$dir/thread-safe-lru/string-key.h" > "$dest/lru-cache-key.h"

