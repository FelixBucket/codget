#include <algorithm>
#include "common.h"

uint32 RefCounted::addref() {
  return InterlockedIncrement(&ref_);
}
uint32 RefCounted::release() {
  if (!this) {
    return 0;
  }
  uint32 result = InterlockedDecrement(&ref_);
  if (!result) {
    delete this;
  }
  return result;
}

void _qmemset(uint32* mem, uint32 fill, uint32 count) {
  while (count--) {
    *mem++ = fill;
  }
}

#include "zlib/zlib.h"
#ifdef _WIN64
  #ifdef _DEBUG
    #pragma comment(lib, "zlib/zlib64d.lib")
  #else
    #pragma comment(lib, "zlib/zlib64r.lib")
  #endif
#else
  #ifdef _DEBUG
    #pragma comment(lib, "zlib/zlib32d.lib")
  #else
    #pragma comment(lib, "zlib/zlib32r.lib")
  #endif
#endif

uint32 gzdeflate(uint8 const* in, uint32 in_size, uint8* out, uint32* out_size) {
  z_stream z;
  memset(&z, 0, sizeof z);
  z.next_in = const_cast<Bytef*>(in);
  z.avail_in = in_size;
  z.total_in = in_size;
  z.next_out = out;
  z.avail_out = *out_size;
  z.total_out = 0;

  memset(out, 0, *out_size);

  int result = deflateInit(&z, Z_DEFAULT_COMPRESSION);
  if (result == Z_OK) {
    result = deflate(&z, Z_FINISH);
    *out_size = z.total_out;
    deflateEnd(&z);
  }
  return (result == Z_STREAM_END ? 0 : -1);
}
uint32 gzencode(uint8 const* in, uint32 in_size, uint8* out, uint32* out_size) {
  z_stream z;
  memset(&z, 0, sizeof z);
  z.next_in = const_cast<Bytef*>(in);
  z.avail_in = in_size;
  z.total_in = in_size;
  z.next_out = out;
  z.avail_out = *out_size;
  z.total_out = 0;

  int result = deflateInit2(&z, 6, Z_DEFLATED, 16 + MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
  if (result == Z_OK) {
    result = deflate(&z, Z_FINISH);
    *out_size = z.total_out;
    deflateEnd(&z);
  }
  return ((result == Z_OK || result == Z_STREAM_END) ? 0 : 1);
}
uint32 gzinflate(uint8 const* in, uint32 in_size, uint8* out, uint32* out_size) {
  z_stream z;
  memset(&z, 0, sizeof z);
  z.next_in = const_cast<Bytef*>(in);
  z.avail_in = in_size;
  z.total_in = in_size;
  z.next_out = out;
  z.avail_out = *out_size;
  z.total_out = 0;

  memset(out, 0, *out_size);

  int result = inflateInit(&z);
  if (result == Z_OK) {
    result = inflate(&z, Z_FINISH);
    *out_size = z.total_out;
    inflateEnd(&z);
  }
  return (z.avail_out == 0 ? 0 : -1);
}
uint32 gzdecode(uint8 const* in, uint32 in_size, uint8* out, uint32* out_size) {
  z_stream z;
  memset(&z, 0, sizeof z);
  z.next_in = const_cast<Bytef*>(in);
  z.avail_in = in_size;
  z.total_in = in_size;
  z.next_out = out;
  z.avail_out = *out_size;
  z.total_out = 0;

  int result = inflateInit2(&z, 16 + MAX_WBITS);
  if (result == Z_OK) {
    result = inflate(&z, Z_FINISH);
    *out_size = z.total_out;
    deflateEnd(&z);
  }
  return (z.avail_out == 0 ? 0 : 1);
}

std::string formatSize(uint64 usize) {
  double size = static_cast<double>(usize);
  if (size < 100 * 1024) {
    return fmtstring("%u B", static_cast<uint32>(size));
  } else if (size < 10.0 * 1024 * 1024) {
    return fmtstring("%u K", static_cast<uint32>(size / 1024));
  } else if (size < 10.0 * 1024 * 1024 * 1024) {
    return fmtstring("%.1f M", size / 1024 / 1024);
  } else {
    return fmtstring("%.1f G", size / 1024 / 1024 / 1024);
  }
}
