#pragma once

#include "wizard.h"
#include "frameui/listctrl.h"

class TagsPage : public Page {
public:
  TagsPage(Wizard* wizard);

  bool hasPrev() const { return true; }
  Page* getPrev();
  bool hasNext() const { return true; }
  Page* getNext();

private:
  StaticFrame* note_;
  StaticFrame* size_;
  ProgressFrame* loading_;
  void addTags(int index, std::string str);
  ButtonFrame* remove_;
  std::vector<ComboFrame*> options_;
  ListFrame* tags_;
  void init();
  LRESULT onMessage(uint32 message, WPARAM wParam, LPARAM lParam);
};
