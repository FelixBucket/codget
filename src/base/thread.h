#pragma once

#include <windows.h>

namespace thread {

  class Waitable {
  protected:
    void* handle;
    Waitable(void* h)
      : handle(h)
    {
    }
  public:
    ~Waitable();

    bool wait(unsigned long timeout = 0xFFFFFFFF);
  };

  class Lock {
    void* impl;
  public:
    Lock();
    ~Lock();
    void acquire();
    void release();

    class Holder {
      Lock* lock;
    public:
      Holder(Lock* l)
        : lock(l)
      {
        if (lock) lock->acquire();
      }
      ~Holder() {
        if (lock) lock->release();
      }
    };
  };
  class Event : public Waitable {
  public:
    Event(bool initial = true);

    void set();
    void reset();
  };
  class Counter : public Waitable {
    long volatile count;
  public:
    Counter();

    operator long() const {
      return count;
    }

    long increment();
    long decrement();
  };

  template<class T>
  struct ThreadArg {
    T* ptr;
    int (T::*func)();
  };
  template<class T>
  DWORD WINAPI ThreadProc(LPVOID arg) {
    ThreadArg<T>* ta = (ThreadArg<T>*) arg;
    int result = (ta->ptr->*(ta->func))();
    delete ta;
    return result;
  }
  template<class T>
  HANDLE create(T* ptr, int (T::*func)(), unsigned long* id = nullptr) {
    ThreadArg<T>* ta = new ThreadArg<T>;
    ta->ptr = ptr;
    ta->func = func;
    return CreateThread(NULL, 0, ThreadProc<T>, ta, 0, id);
  }
  template<class T, class A>
  struct ThreadArg1 {
    T* ptr;
    int (T::*func)(A);
    A arg;
  };
  template<class T, class A>
  DWORD WINAPI ThreadProc1(LPVOID arg) {
    ThreadArg1<T, A>* ta = (ThreadArg1<T, A>*) arg;
    int result = (ta->ptr->*(ta->func))(ta->arg);
    delete ta;
    return result;
  }
  template<class T, class A>
  HANDLE create(T* ptr, int (T::*func)(A), A arg, unsigned long* id = nullptr) {
    ThreadArg1<T, A>* ta = new ThreadArg1<T, A>;
    ta->ptr = ptr;
    ta->func = func;
    ta->arg = arg;
    return CreateThread(NULL, 0, ThreadProc1<T, A>, ta, 0, id);
  }

}
