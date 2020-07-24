#include "error.h"
#include "types.h"
#include <stdarg.h>

Exception::Exception(char const* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  uint32 len = _vscprintf(fmt, ap);
  std::string dst;
  dst.resize(len + 1);
  vsprintf(&dst[0], fmt, ap);
  dst.resize(len);

  buf_.str(dst);
}
