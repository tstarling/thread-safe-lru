Thread-safe LRU Cache
=====================

A set of header-only C++ classes providing thread-safe LRU caches. Uses Intel's
`tbb::concurrent_hash_map`.

* ThreadSafeLRUCache is a single `tbb::concurrent_hash_map` with attached LRU
  list.
* ThreadSafeScalableCache is a cache object built from a collection of
  ThreadSafeLRUCache objects, with items distributed by key hash. This allows
  it to serve a high rate of evictions/inserts without significant lock
  contention.
* ThreadSafeStringKey is a reference-counting string object with a memoized
  hash value. This is recommended as a key object for the above two containers.
