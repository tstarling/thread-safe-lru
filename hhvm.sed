s/namespace tstarling/namespace HPHP/
s/incl_tstarling/incl_HPHP/
s/ThreadSafeStringKey/LRUCacheKey/
s/#include "thread-safe-lru\/lru-cache.h"/#include "hphp\/util\/thread-safe-lru-cache.h"/
s/#include "thread-safe-lru\/scalable-cache.h"/#include "hphp\/util\/thread-safe-scalable-cache.h"/
s/#include "thread-safe-lru\/string-key.h"/#include "hphp\/util\/lru-cache-key.h"/
