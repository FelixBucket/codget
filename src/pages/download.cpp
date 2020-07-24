#include "download.h"
#include "ngdp.h"
#include "app.h"
#include "tags.h"
#include "frameui/fontsys.h"
#include <algorithm>
#include <shlobj.h>

enum {
  ID_BROWSE = 1000,
  ID_START  = 1001,
};

DownloadPage::DownloadPage(Wizard* wizard)
  : Page(wizard)
  , logRead_(0)
{
  StaticFrame* tip = new StaticFrame("Download location:", this);
  tip->setPoint(PT_TOPLEFT, 0, 0);

  browse_ = new ButtonFrame("Browse", this, ID_BROWSE);
  browse_->setPoint(PT_TOP, tip, PT_BOTTOM, 0, 5);
  browse_->setPoint(PT_RIGHT, 0, 0);
  browse_->setSize(80, 21);

  path_ = new EditFrame(this);
  path_->setPoint(PT_TOPLEFT, tip, PT_BOTTOMLEFT, 0, 5);
  path_->setPoint(PT_BOTTOMRIGHT, browse_, PT_BOTTOMLEFT, -8, 0);
  path_->setText(path::root());

  note_ = new StaticFrame("", this);
  note_->setPoint(PT_TOPLEFT, path_, PT_BOTTOMLEFT, 0, 8);

  loading_ = new ProgressFrame(this);
  loading_->setPoint(PT_BOTTOMLEFT, 0, 0);
  loading_->setPoint(PT_BOTTOMRIGHT, 0, 0);
  loading_->setHeight(21);

  files_ = new StaticFrame("X", this);
  files_->setPoint(PT_BOTTOMLEFT, loading_, PT_TOPLEFT, 0, -6);
  files_->setPoint(PT_RIGHT, 0, 0);

  size_ = new StaticFrame("X", this);
  size_->setPoint(PT_BOTTOMLEFT, files_, PT_TOPLEFT, 0, 0);
  size_->setPoint(PT_RIGHT, 0, 0);

  files_->setText("");
  size_->setText("");

  log_ = new EditFrame(this, 0, ES_AUTOHSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY);
  log_->setPoint(PT_TOPLEFT, path_, PT_BOTTOMLEFT, 0, 8);
  log_->setPoint(PT_BOTTOMRIGHT, size_, PT_TOPRIGHT, 0, -8);

  log_->hide();
  loading_->hide();
  size_->hide();
  files_->hide();

  start_ = new ButtonFrame("Start", this, ID_START);
  start_->setPoint(PT_TOPRIGHT, this, PT_BOTTOMRIGHT, 0, 5);
  start_->setSize(100, 21);

  note_->setText("Estimated size: " + formatSize(wizard_->app()->data().downloadSize()));
}

bool browseForFolder(std::wstring prompt, std::wstring& result) {
  IFileDialog* pfd;
  if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd)))) {
    return false;
  }
  DWORD dwOptions;
  if (SUCCEEDED(pfd->GetOptions(&dwOptions))) {
    pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
  }
  pfd->SetTitle(prompt.c_str());
  if (FAILED(pfd->Show(NULL))) {
    pfd->Release();
    return false;
  }
  IShellItem* psi;
  if (FAILED(pfd->GetResult(&psi))) {
    pfd->Release();
    return false;
  }
  wchar_t* str;
  if (FAILED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &str)) || !str) {
    psi->Release();
    pfd->Release();
    return false;
  }
  result = str;
  psi->Release();
  pfd->Release();
  return true;
}

LRESULT DownloadPage::onMessage(uint32 message, WPARAM wParam, LPARAM lParam) {
  auto& data = wizard_->app()->data();
  switch (message) {
  case WM_COMMAND:
    if (LOWORD(wParam) == ID_BROWSE && HIWORD(wParam) == BN_CLICKED) {
      std::wstring path;
      if (browseForFolder(L"Select download location", path)) {
        SetWindowTextW(path_->getHandle(), path.c_str());
      }
    } else if (LOWORD(wParam) == ID_START && HIWORD(wParam) == BN_CLICKED) {
      if (data.progress()) {
        data.stopDownload();
        start_->setText("Start");
        wizard_->enablePrev(true);
        log_->hide();
        loading_->hide();
        size_->hide();
        files_->hide();
        note_->show();
        path_->enable();
        browse_->enable();
      } else {
        logRead_ = 0;
        logText_ = "";
        data.download(path_->getText());
        start_->setText("Cancel");
        wizard_->enablePrev(false);
        log_->show();
        log_->setText("");
        loading_->hide();
        size_->show();
        size_->setText("");
        files_->show();
        files_->setText("");
        note_->hide();
        path_->disable();
        browse_->disable();
      }
    }
    return 0;
  case WM_TASKDONE:
    update(lParam == ProgramData::LOADING);
    if (lParam == 1) {
      loading_->hide();
      start_->setText("Done");
      start_->enable(false);
      wizard_->enablePrev(true);
    }
    return 0;
  default:
    return M_UNHANDLED;
  }
}

void DownloadPage::update(bool loading) {
  auto progress = wizard_->app()->data().progress();
  if (!progress) return;
  if (progress->log.size() > logRead_) {
    while (progress->log.size() > logRead_) {
      if (!logText_.empty()) logText_.append("\r\n");
      logText_.append(progress->log[logRead_++]);
    }
    log_->setText(logText_);
  }
  uint64 sizeDone = progress->sizeDone;
  if (loading) sizeDone += wizard_->app()->data().loading_progress;
  files_->setText(fmtstring("Files: %u/%u", progress->filesDone, progress->filesTotal));
  size_->setText(fmtstring("Downloaded: %s/%s", formatSize(sizeDone).c_str(), formatSize(progress->sizeTotal).c_str()));
  size_->show(progress->sizeTotal != 0);
  if (progress->sizeTotal) {
    loading_->setRange(0, progress->sizeTotal / 1024);
    loading_->setPos(sizeDone / 1024);
  } else {
    loading_->setRange(0, progress->filesTotal);
    loading_->setPos(progress->filesDone);
  }
  loading_->show();
}

Page* DownloadPage::getPrev() {
  return new TagsPage(wizard_);
}
