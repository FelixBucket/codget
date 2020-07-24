#pragma once

template<class Ret, class... Args>
class Functor {
  class FunctionBase {
  public:
    virtual ~FunctionBase() {}
    virtual Ret run(Args... args) const = 0;
  };
  template<class F>
  class Function : public FunctionBase {
    F const& func_;
  public:
    Function(F const& f)
      : func_(f)
    {}
    Ret run(Args... args) const {
      return func_(args...);
    }
  };
  FunctionBase* func_;
public:
  template<class F>
  Functor(F const& f)
    : func_(new Function<F>(f))
  {}
  Functor(Functor const& f) = delete;
  ~Functor() {
    delete func_;
  }
  Functor(Functor&& f)
    : func_(f.func_)
  {
    f.func_ = nullptr;
  }
  Ret operator()(Args... args) const {
    return func_->run(args...);
  }
};

template<class... Args>
class FunctorNoRet {
  class FunctionBase {
  public:
    virtual ~FunctionBase() {}
    virtual void run(Args... args) const = 0;
  };
  template<class F>
  class Function : public FunctionBase {
    F const& func_;
  public:
    Function(F const& f)
      : func_(f)
    {}
    void run(Args... args) const {
      return func_(args...);
    }
  };
  FunctionBase* func_;
public:
  template<class F>
  FunctorNoRet(F const& f)
    : func_(new Function<F>(f))
  {}
  FunctorNoRet(FunctorNoRet const& f) = delete;
  ~FunctorNoRet() {
    delete func_;
  }
  FunctorNoRet(FunctorNoRet&& f)
    : func_(f.func_)
  {
    f.func_ = nullptr;
  }
  void operator()(Args... args) const {
    func_->run(args...);
  }
};
