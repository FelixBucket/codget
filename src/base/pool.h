#pragma once

#include "types.h"

class FixedMemoryPool {
public:
  FixedMemoryPool(uint32 itemSize, uint32 poolGrow = 65536);
  FixedMemoryPool(FixedMemoryPool&& src)
    : itemSize(src.itemSize)
    , chunkSize(src.chunkSize)
    , chunks(src.chunks)
    , freeChunks(src.freeChunks)
  {
    src.chunks = nullptr;
    src.freeChunks = nullptr;
  }
  FixedMemoryPool(FixedMemoryPool const& src) = delete;
  ~FixedMemoryPool();

  void* alloc();
  void free(void* ptr);

  void clear();

private:
  struct MemoryChunk;
  uint32 itemSize;
  uint32 chunkSize;
  MemoryChunk* chunks;
  MemoryChunk* freeChunks;
};
template<class T>
class TypedMemoryPool : private FixedMemoryPool
{
public:
  TypedMemoryPool(uint32 poolGrow = 65536)
    : FixedMemoryPool(sizeof(T), poolGrow)
  {
  }
  TypedMemoryPool(TypedMemoryPool&& src)
    : FixedMemoryPool(std::move(src))
  {
  }
  TypedMemoryPool(TypedMemoryPool const& src) = delete;

  T* alloc() {
    T* ptr = (T*)FixedMemoryPool::alloc();
    new(ptr)T();
    return ptr;
  }
  void free(T* ptr) {
    ptr->~T();
    FixedMemoryPool::free(ptr);
  }
};
