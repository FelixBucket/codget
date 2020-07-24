#pragma once

#include "types.h"
#include <string>
#include <sstream>
#include <cctype>
#include <vector>
#include <map>
#define NOMINMAX
#include <windows.h>

#include "string.h"

class RefCounted {
  uint32 ref_;
public:
  RefCounted() : ref_(1) {}
  virtual ~RefCounted() {}

  bool unique() const {
    return ref_ == 1;
  }
  uint32 addref();
  uint32 release();
};

void _qmemset(uint32* mem, uint32 fill, uint32 count);

template<class To>
using Map = std::map<istring, To>;
typedef Map<std::string> Dictionary;

template<class MapType, class Key>
typename MapType::mapped_type* getptr(MapType& map, Key const& key) {
  auto it = map.find(key);
  return (it == map.end() ? nullptr : &it->second);
}
template<class MapType, class Key>
typename MapType::mapped_type const* getptr(MapType const& map, Key const& key) {
  auto it = map.find(key);
  return (it == map.end() ? nullptr : &it->second);
}

uint32 gzdeflate(uint8 const* in, uint32 in_size, uint8* out, uint32* out_size);
uint32 gzencode(uint8 const* in, uint32 in_size, uint8* out, uint32* out_size);
uint32 gzinflate(uint8 const* in, uint32 in_size, uint8* out, uint32* out_size);
uint32 gzdecode(uint8 const* in, uint32 in_size, uint8* out, uint32* out_size);

template<int TS>
struct FlipTraits {};

template<> struct FlipTraits<1> {
  typedef unsigned char T;
  static T flip(T x) { return x; }
};

template<> struct FlipTraits<2> {
  typedef unsigned short T;
  static T flip(T x) { return _byteswap_ushort(x); }
};

template<> struct FlipTraits<4> {
  typedef unsigned long T;
  static T flip(T x) { return _byteswap_ulong(x); }
};

template<> struct FlipTraits<8> {
  typedef unsigned long long T;
  static T flip(T x) { return _byteswap_uint64(x); }
};

template<typename T>
void flip(T& x) {
  typedef FlipTraits<sizeof(T)> Flip;
  x = static_cast<T>(Flip::flip(static_cast<Flip::T>(x)));
}

std::string formatSize(uint64 size);
