#pragma once

#include "wizard.h"

class BuildPage : public Page {
public:
  BuildPage(Wizard* wizard);

  bool hasPrev() const { return true; }
  Page* getPrev();
  bool hasNext() const { return true; }
  Page* getNext();

private:
  ComboFrame* build_;
  EditFrame* tags_;
  void init();
  void showInfo(std::string const& build);
  LRESULT onMessage(uint32 message, WPARAM wParam, LPARAM lParam);
};
