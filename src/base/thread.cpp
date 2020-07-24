#include "thread.h"
#include <windows.h>

namespace thread {

  Waitable::~Waitable() {
    CloseHandle(handle);
  }
  bool Waitable::wait(unsigned long timeout) {
    return WaitForSingleObject(handle, timeout) == WAIT_OBJECT_0;
  }

  Lock::Lock() {
    CRITICAL_SECTION* cs = new CRITICAL_SECTION;
    InitializeCriticalSection(cs);
    impl = cs;
  }
  Lock::~Lock() {
    DeleteCriticalSection((CRITICAL_SECTION*)impl);
  }
  void Lock::acquire() {
    EnterCriticalSection((CRITICAL_SECTION*)impl);
  }
  void Lock::release() {
    LeaveCriticalSection((CRITICAL_SECTION*)impl);
  }

  Event::Event(bool initial)
    : Waitable(CreateEvent(NULL, TRUE, initial ? TRUE : FALSE, NULL))
  {
  }
  void Event::set() {
    SetEvent(handle);
  }
  void Event::reset() {
    ResetEvent(handle);
  }

  Counter::Counter()
    : Waitable(CreateEvent(NULL, TRUE, TRUE, NULL))
    , count(0)
  {
  }

  long Counter::increment() {
    long result = InterlockedIncrement(&count);
    if (result > 0) ResetEvent(handle);
    return result;
  }
  long Counter::decrement() {
    long result = InterlockedDecrement(&count);
    if (result <= 0) SetEvent(handle);
    return result;
  }

}
