while (<>) {
	s/namespace tstarling/namespace HPHP/g;
	s/incl_tstarling/incl_HPHP/g;
	s/ThreadSafeStringKey/LRUCacheKey/g;
	s/#include "thread-safe-lru\/lru-cache.h"/#include "hphp\/util\/thread-safe-lru-cache.h"/g;
	s/#include "thread-safe-lru\/scalable-cache.h"/#include "hphp\/util\/thread-safe-scalable-cache.h"/g;
	s/#include "thread-safe-lru\/string-key.h"/#include "hphp\/util\/lru-cache-key.h"/g;
	s/#include "thread-safe-lru\/hash.h"/#include "hphp\/util\/hash.h"/g;
	print;
}
