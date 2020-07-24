#include "wizard.h"
#include "app.h"

#include "resource.h"

enum {
  ID_BUTTON_PREV    = 7000,
  ID_BUTTON_NEXT    = 7001,
};

Wizard::Wizard(Application* app)
  : app_(app)
  , page_(nullptr)
  , buttonPrev_(nullptr)
  , buttonNext_(nullptr)
  , hIcon(LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_BATTLE_130)))
{
  if (WNDCLASSEX* wcx = createclass("MainWndClass")) {
    wcx->hbrBackground = HBRUSH(COLOR_BTNFACE + 1);
    wcx->hCursor = LoadCursor(NULL, IDC_ARROW);
    wcx->hIcon = hIcon;
    RegisterClassEx(wcx);
  }
  create(CW_USEDEFAULT, 0, 500, 400, "Battle.Net Call of Duty Downloader",
    (WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX)) | WS_CLIPCHILDREN, WS_EX_CONTROLPARENT);

  buttonPrev_ = new ButtonFrame("Back", this, ID_BUTTON_PREV);
  buttonPrev_->setSize(100, 21);
  buttonPrev_->setPoint(PT_BOTTOMLEFT, 10, -10);
  buttonPrev_->hide();
  buttonNext_ = new ButtonFrame("Next", this, ID_BUTTON_NEXT);
  buttonNext_->setSize(100, 21);
  buttonNext_->setPoint(PT_BOTTOMRIGHT, -10, -10);
  buttonNext_->hide();

  showWindow();
}

void Wizard::setPage(Page* page) {
  app_->data().stop();
  delete page_;
  page_ = page;
  page->setPoint(PT_TOPLEFT, 10, 10);
  page->setPoint(PT_BOTTOMRIGHT, buttonNext_, PT_TOPRIGHT, 0, -5);
  buttonPrev_->enable(true);
  buttonNext_->enable(true);
  buttonPrev_->show(page->hasPrev());
  buttonNext_->show(page->hasNext());
  page->show();
  page->init();
}

LRESULT Wizard::onMessage(uint32 message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
  case WM_COMMAND:
    if (LOWORD(wParam) == ID_BUTTON_PREV && HIWORD(wParam) == BN_CLICKED) {
      if (page_ && page_->hasPrev()) {
        Page* page = page_->getPrev();
        if (page) setPage(page);
      }
    } else if (LOWORD(wParam) == ID_BUTTON_NEXT && HIWORD(wParam) == BN_CLICKED) {
      if (page_ && page_->hasNext()) {
        Page* page = page_->getNext();
        if (page) setPage(page);
      }
    }
    return 0;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  case WM_TASKDONE:
    if (page_ == (Page*) wParam) {
      page_->onMessage(WM_TASKDONE, 0, lParam);
    }
    return 0;
  default:
    return M_UNHANDLED;
  }
  return 0;
}
