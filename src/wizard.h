#pragma once

#include "frameui/framewnd.h"
#include "frameui/controlframes.h"

class Application;
class Page;

class Wizard : public RootWindow {
public:
  Wizard(Application* app);

  Page* page() {
    return page_;
  }
  void setPage(Page* page);

  void enablePrev(bool enable) {
    buttonPrev_->enable(enable);
  }
  void enableNext(bool enable) {
    buttonNext_->enable(enable);
  }

  Application* app() const {
    return app_;
  }

private:
  HICON hIcon;
  Application* app_;
  Page* page_;
  ButtonFrame* buttonPrev_;
  ButtonFrame* buttonNext_;
  LRESULT onMessage(uint32 message, WPARAM wParam, LPARAM lParam);
};

class Page : public Frame {
public:
  Page(Wizard* wizard)
    : Frame(wizard)
    , wizard_(wizard)
  {
    hide();
  }

  virtual void init() {}
  virtual bool hasNext() const { return false; }
  virtual bool hasPrev() const { return false; }
  virtual Page* getNext() { return nullptr; }
  virtual Page* getPrev() { return nullptr; }

protected:
  Wizard* wizard_;
};
