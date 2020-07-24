#pragma once

#include <windows.h>
#include <memory>
#include "frameui/window.h"
#include "ngdp.h"
#include "data.h"

class Wizard;

class Application {
public:
  Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
  ~Application();

  int run();

  ProgramData& data() {
    return data_;
  }
  Wizard* wizard() {
    return wizard_;
  }

private:
  ProgramData data_;
  Wizard* wizard_;
};
