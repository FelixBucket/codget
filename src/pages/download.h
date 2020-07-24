#pragma once

#include "wizard.h"

class DownloadPage : public Page {
public:
  DownloadPage(Wizard* wizard);

  bool hasPrev() const { return true; }
  Page* getPrev();

private:
  EditFrame* path_;
  ButtonFrame* start_;
  ButtonFrame* browse_;

  StaticFrame* note_;

  EditFrame* log_;
  StaticFrame* files_;
  StaticFrame* size_;
  ProgressFrame* loading_;
  size_t logRead_;
  std::string logText_;

  LRESULT onMessage(uint32 message, WPARAM wParam, LPARAM lParam);
  void update(bool loading);
};
