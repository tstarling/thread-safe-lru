while (<>) {
	s/namespace tstarling/namespace HPHP/g;
	s/incl_tstarling/incl_HPHP_UTIL/g;
	s/incl_HPHP_UTIL_STRING_KEY_H/incl_HPHP_UTIL_LRU_CACHE_KEY_H/g;
	s/ThreadSafeScalableCache/ConcurrentScalableCache/g;
	s/ThreadSafeLRUCache/ConcurrentLRUCache/g;
	s/ThreadSafeStringKey/LRUCacheKey/g;
	s/#include "thread-safe-lru\/lru-cache.h"/#include "hphp\/util\/concurrent-lru-cache.h"/g;
	s/#include "thread-safe-lru\/scalable-cache.h"/#include "hphp\/util\/concurrent-scalable-cache.h"/g;
	s/#include "thread-safe-lru\/string-key.h"/#include "hphp\/util\/lru-cache-key.h"/g;
	s/#include "thread-safe-lru\/hash.h"/#include "hphp\/util\/hash.h"/g;
	print;
}
