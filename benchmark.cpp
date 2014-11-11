/*
 * Copyright © 2014 Tim Starling
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "thread-safe-lru/string-key.h"
#include "thread-safe-lru/scalable-cache.h"
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <thread>
#include <memory>
#include <random>
#include <atomic>
#include <chrono>

typedef tstarling::ThreadSafeStringKey String;
typedef String::HashCompare HashCompare;
typedef tstarling::ThreadSafeScalableCache<String, int, HashCompare> ScalableCache;
typedef tstarling::ThreadSafeLRUCache<String, int, HashCompare> AtomicCache;
std::unique_ptr<AtomicCache> aecm;
std::unique_ptr<ScalableCache> secm;
std::vector<String> strings;
std::atomic<long> globalHitCount;
std::atomic<long> globalTotalCount;
volatile bool stop = false;

enum {
  LRU,
  SCALABLE
} cacheType;

void threadMain();

int main(int argc, char ** argv) {
  using std::chrono::steady_clock;
  using std::chrono::seconds;
  using std::chrono::duration_cast;
  using std::chrono::duration;

  if (argc < 5) {
    std::cerr << "Usage: " << argv[0] << " <lru|scalable> <threads> <cache-size> <demand-size> [<duration>]\n";
    return 1;
  }

  if (0 == strcmp(argv[1], "lru")) {
    cacheType = LRU;
  } else if (0 == strcmp(argv[1], "scalable")) {
    cacheType = SCALABLE;
  } else {
    std::cerr << "Cache type must be either \"lru\" or \"scalable\"\n";
    return 1;
  }

  int numThreads = boost::lexical_cast<int>(argv[2]);
  int cacheSize = boost::lexical_cast<int>(argv[3]);
  int demandSize = boost::lexical_cast<int>(argv[4]);
  int durationLimit = 0;

  if (argc > 5) {
    durationLimit = boost::lexical_cast<int>(argv[5]);
  }

  aecm = std::unique_ptr<AtomicCache>(new AtomicCache(cacheSize));
  secm = std::unique_ptr<ScalableCache>(new ScalableCache(cacheSize));
  for (int i = 0; i < demandSize; i++) {
    std::string s = std::string(100, 'x') + boost::lexical_cast<std::string>(i);
    strings.push_back(String(s.data(), s.size()));
  }

  std::vector<std::thread> threads;
  threads.reserve(numThreads);
  auto startTime = steady_clock::now();
  auto oneSecond = seconds(1);

  for (int i = 0; i < numThreads; i++) {
    threads.push_back(std::thread(threadMain));
  }
  if (durationLimit == 0) {
    long prevTotalCount = 0, prevHitCount = 0;
    for (;;) {
      std::this_thread::sleep_for(oneSecond);
      long totalCount = globalTotalCount.load();
      long hitCount = globalHitCount.load();
      printf("rate = %.5g kreq/s, hit ratio = %.3g%%\n",
          (totalCount - prevTotalCount) / 1000.,
          (double)(hitCount - prevHitCount) / (totalCount - prevTotalCount) * 100);

      prevTotalCount = totalCount;
      prevHitCount = hitCount;
    }
  } else {
    std::this_thread::sleep_for(oneSecond * durationLimit);
    long totalCount = globalTotalCount.load();
    long hitCount = globalHitCount.load();
    auto totalTime = duration_cast<duration<double>>(
        steady_clock::now() - startTime);
    stop = true;
    for (int i = 0; i < numThreads; i++) {
      threads[i].join();
    }
    printf("type\tthreads\tcache\tdemand\tduration\trate\tratio\n");
    printf("%s\t%d\t%d\t%d\t%g\t%g\t%g%%\n",
        cacheType == LRU ? "lru" : "scal",
        numThreads,
        cacheSize,
        demandSize,
        totalTime.count(),
        totalCount / 1000. / totalTime.count(),
        (double)hitCount / totalCount * 100);
  }
  return 0;
}

void threadMain() {
  int cacheSize = strings.size();
  
  std::mt19937 generator(std::hash<pthread_t>()(pthread_self()));
  std::uniform_int_distribution<int> rand(0, cacheSize - 1);

  if (cacheType == LRU) {
    AtomicCache::ConstAccessor ac;
    while (!stop) {
      long hitCount = 0;
      long totalCount = 0;
      for (int j = 0; j < 1000; j++) {
        int r = rand(generator);
        if (aecm->find(ac, strings[r])) {
          hitCount++;
        } else {
          aecm->insert(strings[r], j);
        }
        totalCount++;
      }
      globalHitCount += hitCount;
      globalTotalCount += totalCount;
    }
  } else {
    ScalableCache::ConstAccessor ac;
    while (!stop) {
      long hitCount = 0;
      long totalCount = 0;
      for (int j = 0; j < 1000; j++) {
        int r = rand(generator);
        if (secm->find(ac, strings[r])) {
          hitCount++;
        } else {
          secm->insert(strings[r], j);
        }
        totalCount++;
      }
      globalHitCount += hitCount;
      globalTotalCount += totalCount;
    }
  }
}

