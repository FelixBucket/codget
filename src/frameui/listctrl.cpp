#include "frameui/framewnd.h"
#include "frameui/fontsys.h"
#include "listctrl.h"

ListFrame::ListFrame(Frame* parent, int id, int style, int styleEx)
  : WindowFrame(parent)
{
  create(WC_LISTVIEW, "", WS_CHILD | WS_TABSTOP | style, WS_EX_CLIENTEDGE);
  setFont(FontSys::getSysFont());
  setId(id);
  ListView_SetExtendedListViewStyle(hWnd, styleEx);
}

void ListFrame::clear() {
  ListView_DeleteAllItems(hWnd);
}
int ListFrame::addItem(std::string const& name, int data) {
  LVITEM lvi;
  memset(&lvi, 0, sizeof lvi);
  lvi.iItem = ListView_GetItemCount(hWnd);
  lvi.mask = LVIF_IMAGE | LVIF_TEXT | LVIF_PARAM;
  lvi.pszText = const_cast<char*>(name.c_str());
  lvi.lParam = data;
  ListView_InsertItem(hWnd, &lvi);
  return lvi.iItem;
}

int ListFrame::getCount() const {
  return ListView_GetItemCount(hWnd);
}

int ListFrame::getItemData(int item) {
  LVITEM lvi;
  memset(&lvi, 0, sizeof lvi);
  lvi.iItem = item;
  lvi.mask = LVIF_PARAM;
  ListView_GetItem(hWnd, &lvi);
  return lvi.lParam;
}
