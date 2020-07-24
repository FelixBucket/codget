#pragma once

#include "wizard.h"

class ProgramPage : public Page {
public:
  ProgramPage(Wizard* wizard);

  bool hasNext() const { return true; }
  Page* getNext();

private:
  ComboFrame* program_;
  ComboFrame* region_;
  void init();
  LRESULT onMessage(uint32 message, WPARAM wParam, LPARAM lParam);

  std::vector<std::string> programs_;
  std::vector<std::string> regions_;
};
