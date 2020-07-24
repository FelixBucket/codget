#include "pool.h"

struct FixedMemoryPool::MemoryChunk {
  MemoryChunk* next;
  MemoryChunk* nextFree;

  uint8* begin;
  uint8* end;
  uint8* firstFree;
  uint8* firstUnused;
  uint32 itemSize;

  MemoryChunk(uint32 size, uint32 count);
  ~MemoryChunk();

  bool hasSpace() {
    return firstFree || firstUnused < end;
  }
  bool contains(uint8* ptr) {
    return ptr >= begin && ptr < end;
  }
  uint8* alloc();
  void free(uint8* ptr);
};

FixedMemoryPool::MemoryChunk::MemoryChunk(uint32 size, uint32 count)
  : next(nullptr)
  , nextFree(nullptr)
  , itemSize(size)
  , firstFree(nullptr)
{
  begin = new uint8[size * count];
  end = begin + size * count;
  firstUnused = begin;
}
FixedMemoryPool::MemoryChunk::~MemoryChunk() {
  delete[] begin;
}

uint8* FixedMemoryPool::MemoryChunk::alloc() {
  if (firstFree) {
    uint8* result = firstFree;
    firstFree = *(uint8**)firstFree;
    return result;
  } else if (firstUnused < end) {
    uint8* result = firstUnused;
    firstUnused += itemSize;
    return result;
  } else {
    return nullptr;
  }
}
void FixedMemoryPool::MemoryChunk::free(uint8* ptr) {
  *(uint8**)ptr = firstFree;
  firstFree = ptr;
}

FixedMemoryPool::FixedMemoryPool(uint32 itemSize, uint32 poolGrow)
  : itemSize(itemSize)
  , chunkSize(poolGrow / itemSize)
  , chunks(nullptr)
  , freeChunks(nullptr)
{
}
FixedMemoryPool::~FixedMemoryPool() {
  clear();
}

void* FixedMemoryPool::alloc() {
  if (freeChunks == nullptr) {
    freeChunks = new MemoryChunk(itemSize, chunkSize);
    freeChunks->next = chunks;
    chunks = freeChunks;
  }
  void* ptr = freeChunks->alloc();
  if (!freeChunks->hasSpace()) {
    freeChunks = freeChunks->nextFree;
  }
  return ptr;
}
void FixedMemoryPool::free(void* ptr) {
  for (MemoryChunk* chunk = chunks; chunk; chunk = chunk->next) {
    if (chunk->contains((uint8*)ptr)) {
      if (!chunk->hasSpace()) {
        chunk->nextFree = freeChunks;
        freeChunks = chunk;
      }
      chunk->free((uint8*)ptr);
      return;
    }
  }
}
void FixedMemoryPool::clear() {
  while (chunks) {
    MemoryChunk* next = chunks->next;
    delete chunks;
    chunks = next;
  }
  freeChunks = nullptr;
}
