#!/bin/bash

if [ -z "$1" ]; then
	echo "Usage: $0 <hhvm-dir>"
	exit 1
fi

dir=`dirname $0`
dest="$1/hphp/util"
perl $dir/hhvm.pl "$dir/thread-safe-lru/lru-cache.h" > "$dest/concurrent-lru-cache.h"
perl $dir/hhvm.pl "$dir/thread-safe-lru/scalable-cache.h" > "$dest/concurrent-scalable-cache.h"
perl $dir/hhvm.pl "$dir/thread-safe-lru/string-key.h" > "$dest/lru-cache-key.h"
