#include "app.h"
#include <commctrl.h>
#include "wizard.h"
#include "pages/program.h"

Application::Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
  : wizard_(nullptr)
  , data_(this)
{
  INITCOMMONCONTROLSEX iccex;
  iccex.dwSize = sizeof iccex;
  iccex.dwICC = ICC_STANDARD_CLASSES | ICC_PROGRESS_CLASS |
    ICC_BAR_CLASSES | ICC_TREEVIEW_CLASSES | ICC_LISTVIEW_CLASSES |
    ICC_TAB_CLASSES | ICC_UPDOWN_CLASS | ICC_DATE_CLASSES;
  InitCommonControlsEx(&iccex);
  OleInitialize(NULL);

  wizard_ = new Wizard(this);
  wizard_->setPage(new ProgramPage(wizard_));
}

Application::~Application() {
  delete wizard_;

  OleFlushClipboard();
  OleUninitialize();
}

int Application::run() {
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}
