#pragma once

#include "framewnd.h"
#include "controlframes.h"

class Scrollable : public WindowFrame {
public:
  Scrollable(Frame* parent)
    : WindowFrame(parent)
    , scrollPos(0)
    , scrollAccum(0)
  {}

protected:
  int scrollPos;
  int scrollAccum;
  void scroll(int pos);
  LRESULT onMessage(uint32 message, WPARAM wParam, LPARAM lParam) override;
};
