#pragma once

#include "frameui/frame.h"
#include "frameui/window.h"
#include "frameui/framewnd.h"

#include <commctrl.h>

class ListFrame : public WindowFrame {
public:
  ListFrame(Frame* parent, int id = 0,
    int style = LVS_LIST | LVS_SHOWSELALWAYS,
    int styleEx = 0);

  void clear();

  int getItemData(int item);
  int getCount() const;
  int addItem(std::string const& name, int data = 0);

  int getCurSel() const {
    return ListView_GetNextItem(hWnd, -1, LVNI_SELECTED);
  }
};
