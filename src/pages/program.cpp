#include "program.h"
#include "ngdp.h"
#include "app.h"
#include "build.h"

enum {
  ID_PROGRAM      = 1000,
  ID_REGION       = 1001,
  ID_LINK         = 1002,
  ID_LINK_2         = 1003,
};

ProgramPage::ProgramPage(Wizard* wizard)
  : Page(wizard)
{
  StaticFrame* tip = new StaticFrame("Welcome to CodGet", this);
  StaticFrame* tip4 = new StaticFrame("", this);
  tip->setPoint(PT_TOPLEFT, 0, 0);
  tip4->setPoint(PT_BOTTOM, 0, 0);


  StaticFrame* tip2 = new StaticFrame("This tool can download any Battle.Net supported Call of Duty game from the Blizzard game CDN", this);
  tip2->setPoint(PT_TOPLEFT, tip, PT_BOTTOMLEFT, 0, 0);
  tip = new StaticFrame("For bugs and suggestions, go to", this);
  tip->setPoint(PT_TOPLEFT, tip2, PT_BOTTOMLEFT, 0, 0);
  LinkFrame* link = new LinkFrame("https://github.com/FelixBucket/codget", this, ID_LINK);
  link->setPoint(PT_TOPLEFT, tip, PT_BOTTOMLEFT, 0, 0);



  StaticFrame* tip3 = new StaticFrame("Original code for BlizzGet can be found here:", this);
  tip3->setPoint(PT_BOTTOMRIGHT, tip4, PT_TOPRIGHT, 0, 0);
  LinkFrame* link2 = new LinkFrame("https://github.com/d07RiV/blizzget", this, ID_LINK_2);
  link2->setPoint(PT_TOPRIGHT, tip3, PT_BOTTOMRIGHT, 0, 0);

  program_ = new ComboFrame(this, ID_PROGRAM);
  for (auto const& kv : NGDP::ProgramCodes) {
    programs_.push_back(kv.first);
    program_->addString(kv.first + " - " + kv.second);
  }
  program_->setPoint(PT_TOPLEFT, link, PT_BOTTOMLEFT, 120, 16);
  program_->setPoint(PT_RIGHT, 0, 0);
  ComboBox_SetCueBannerText(program_->getHandle(), L"Select one");
  StaticFrame::addTip(program_, "Program code:");

  region_ = new ComboFrame(this, ID_REGION);
  region_->setPoint(PT_TOPLEFT, program_, PT_BOTTOMLEFT, 0, 8);
  region_->setPoint(PT_RIGHT, 0, 0);
  region_->addString("Select program first");
  region_->setCurSel(0);
  region_->disable();
  ComboBox_SetCueBannerText(region_->getHandle(), L"Select region");
  StaticFrame::addTip(region_, "Region:");
}
void ProgramPage::init() {
  wizard_->enableNext(false);
  auto ngdp = wizard_->app()->data().ngdp();
  if (ngdp) {
    auto it = std::find(programs_.begin(), programs_.end(), ngdp->program());
    if (it == programs_.end()) return;
    program_->setCurSel(it - programs_.begin());
    regions_ = wizard_->app()->data().ngdp()->regions();
    region_->reset();
    for (std::string const& r : regions_) {
      region_->addString(r);
    }
    it = std::find(regions_.begin(), regions_.end(), ngdp->region());
    region_->setCurSel(it == regions_.end() ? -1 : it - regions_.begin());
    region_->enable();
    wizard_->enableNext(ngdp->version());
  }
}

LRESULT ProgramPage::onMessage(uint32 message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
  case WM_COMMAND:
    if (LOWORD(wParam) == ID_PROGRAM && HIWORD(wParam) == CBN_SELCHANGE) {
      int index = program_->getCurSel();
      region_->reset();
      region_->addString("Loading...");
      region_->setCurSel(0);
      region_->disable();
      wizard_->app()->data().setProgram(index >= 0 && index < programs_.size() ? programs_[index] : "");
      wizard_->enableNext(false);
    } else if (LOWORD(wParam) == ID_REGION && HIWORD(wParam) == CBN_SELCHANGE) {
      int index = region_->getCurSel();
      auto ngdp = wizard_->app()->data().ngdp();
      if (ngdp && index >= 0 && index < regions_.size()) {
        wizard_->enableNext(ngdp->setRegion(regions_[index]));
      } else {
        wizard_->enableNext(false);
      }
    } else if (LOWORD(wParam) == ID_LINK && HIWORD(wParam) == BN_CLICKED) {
      ShellExecute(NULL, "open", "https://github.com/FelixBucket/codget", NULL, NULL, SW_SHOWNORMAL);
    } else if (LOWORD(wParam) == ID_LINK_2 && HIWORD(wParam) == BN_CLICKED) {
      ShellExecute(NULL, "open", "https://github.com/d07RiV/blizzget", NULL, NULL, SW_SHOWNORMAL);
    }
    return 0;
  case WM_TASKDONE:
    if (lParam == ProgramData::LOADING) return 0;
    if (!wizard_->app()->data().ngdp()) {
      region_->reset();
      region_->addString("Failed to load region list");
      region_->setCurSel(0);
      region_->disable();
      return 0;
    }
    regions_ = wizard_->app()->data().ngdp()->regions();
    region_->reset();
    for (std::string const& r : regions_) {
      region_->addString(r);
    }
    region_->setCurSel(0);
    wizard_->enableNext(wizard_->app()->data().ngdp()->setRegion(regions_[0]));
    region_->enable();
    return 0;
  default:
    return M_UNHANDLED;
  }
}

Page* ProgramPage::getNext() {
  auto ngdp = wizard_->app()->data().ngdp();
  if (ngdp && ngdp->version()) {
    return new BuildPage(wizard_);
  } else {
    return nullptr;
  }
}
