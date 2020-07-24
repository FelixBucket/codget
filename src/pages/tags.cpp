#include "tags.h"
#include "ngdp.h"
#include "app.h"
#include "build.h"
#include "download.h"
#include "frameui/fontsys.h"
#include <algorithm>

enum {
  ID_ADD = 1000,
  ID_REMOVE = 1001,
};

TagsPage::TagsPage(Wizard* wizard)
  : Page(wizard)
  , tags_(nullptr)
  , remove_(nullptr)
{
  note_ = new StaticFrame("Fetching encoding table...", this);
  note_->setPoint(PT_TOPLEFT, 32, 0);
  loading_ = new ProgressFrame(this);
  loading_->setPoint(PT_TOPLEFT, note_, PT_BOTTOMLEFT, 0, 8);
  loading_->setPoint(PT_RIGHT, -32, 0);
  loading_->setHeight(21);
  loading_->hide();
}

void TagsPage::init() {
  wizard_->enableNext(false);
  wizard_->app()->data().loadTags();
}

void TagsPage::addTags(int index, std::string str) {
  auto& data = wizard_->app()->data();
  if (index >= options_.size()) {
    tags_->addItem(str);
    data.used_tags.push_back(str);
  } else {
    //if (index) str.push_back(' ');
    int sel = options_[index]->getCurSel();
    if (sel >= 0) sel = options_[index]->getItemData(sel);
    if (sel < 0) {
      //for (auto const& tag : data.tags[index]) {
      //  addTags(index + 1, str + tag);
      //}
      addTags(index + 1, str);
    } else {
      std::string next = str;
      if (!str.empty()) next.push_back(' ');
      addTags(index + 1, next + data.tags[index][sel]);
    }
  }
}

LRESULT TagsPage::onMessage(uint32 message, WPARAM wParam, LPARAM lParam) {
  auto& data = wizard_->app()->data();
  switch (message) {
  case WM_NOTIFY:
    if (((NMHDR*) lParam)->code == LVN_ITEMCHANGED) {
      int sel = tags_->getCurSel();
      remove_->enable(sel >= 0);
    }
    return 0;
  case WM_COMMAND:
    if (LOWORD(wParam) == ID_ADD && HIWORD(wParam) == BN_CLICKED) {
      addTags(0, "");
      size_->setText("Total size: " + formatSize(data.downloadSize()));
    } else if (LOWORD(wParam) == ID_REMOVE && HIWORD(wParam) == BN_CLICKED) {
      int pos;
      while ((pos = tags_->getCurSel()) >= 0) {
        ListView_DeleteItem(tags_->getHandle(), pos);
        data.used_tags.erase(data.used_tags.begin() + pos);
      }
      size_->setText("Total size: " + formatSize(data.downloadSize()));
    }
    return 0;
  case WM_TASKDONE:
    if (lParam == -1) {
      loading_->hide();
      note_->setText("Failed to load build tags");
    } else if (lParam == ProgramData::LOADING + 0 || lParam == ProgramData::LOADING + 1) {
      if (lParam == ProgramData::LOADING + 0) {
        note_->setText("Fetching encoding table...");
      } else {
        note_->setText("Fetching download list...");
      }
      loading_->setRange(0, data.loading_size);
      loading_->setPos(data.loading_progress);
      loading_->show(data.loading_progress < data.loading_size);
    } else {
      loading_->hide();
      note_->setText("Select build tags:");
      int curWidth = 1000;
      Frame* prevBox = note_;
      Frame* prevLine = note_;
      for (auto const& tags : data.tags) {
        int maxWidth = std::max<int>(60, FontSys::getTextSize(FontSys::getSysFont(), "Any").cx);
        ComboFrame* box = new ComboFrame(this);
        options_.push_back(box);
        if (tags.size() > 0) {
          box->addString("Any", -1);
        }
        for (size_t i = 0; i < tags.size(); ++i) {
          box->addString(tags[i], i);
          maxWidth = std::max<int>(maxWidth, FontSys::getTextSize(FontSys::getSysFont(), tags[i]).cx);
        }
        box->setCurSel(0);
        box->setWidth(maxWidth + 20);
        if (curWidth + maxWidth + 28 > 300) {
          box->setPoint(PT_TOPLEFT, prevLine, PT_BOTTOMLEFT, 0, 8);
          prevLine = box;
          curWidth = maxWidth + 20;
        } else {
          box->setPoint(PT_BOTTOMLEFT, prevBox, PT_BOTTOMRIGHT, 8, 0);
          curWidth += maxWidth + 28;
        }
        prevBox = box;
      }

      ButtonFrame* btn = new ButtonFrame("+", this, ID_ADD);
      btn->setSize(21, 21);
      btn->setPoint(PT_TOPRIGHT, note_, PT_BOTTOMLEFT, -8, 8);

      size_ = new StaticFrame("Total size", this);
      size_->setPoint(PT_LEFT, note_, PT_LEFT, 0, 0);
      size_->setPoint(PT_BOTTOMRIGHT, 0, 0);

      tags_ = new ListFrame(this);
      tags_->setPoint(PT_TOPLEFT, prevLine, PT_BOTTOMLEFT, 0, 8);
      tags_->setPoint(PT_BOTTOMRIGHT, size_, PT_TOPRIGHT, 0, -8);

      remove_ = new ButtonFrame("-", this, ID_REMOVE);
      remove_->setSize(21, 21);
      remove_->setPoint(PT_TOPRIGHT, tags_, PT_TOPLEFT, -8, 0);
      remove_->disable();

      if (data.used_tags.size()) {
        for (std::string const& tag : data.used_tags) {
          tags_->addItem(tag);
        }
      }
      size_->setText("Total size: " + formatSize(data.downloadSize()));
      wizard_->enableNext(true);
    }
    return 0;
  default:
    return M_UNHANDLED;
  }
}

Page* TagsPage::getPrev() {
  return new BuildPage(wizard_);
}
Page* TagsPage::getNext() {
  return new DownloadPage(wizard_);
}
