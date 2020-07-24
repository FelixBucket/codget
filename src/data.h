#pragma once

#include <memory>
#include "ngdp.h"

#define WM_TASKDONE     (WM_USER+1928)

class Application;
class Page;

class ProgramData {

public:
  void stop() {
    setTask(nullptr);
  }

  enum { LOADING = -123 };

  uint32 loading_size;
  uint32 loading_progress;

  // step 1

public:
  void setProgram(std::string const& program);
  std::shared_ptr<NGDP::NGDP> ngdp() {
    return ngdp_;
  }
private:
  std::shared_ptr<NGDP::NGDP> ngdp_;

  // step 2

public:
  void loadBuilds();
  void loadBuild(std::string const& build);
  NGDP::ConfigFile cdn_config;
  std::vector<std::string> builds;
  std::map<std::string, NGDP::ConfigFile> build_configs;
  std::string builds_loaded;
  std::string selected_build;

  // step 3

public:
  void loadTags();
  std::vector<std::vector<std::string>> tags;
  std::vector<std::string> used_tags;
  uint64 downloadSize();

private:
  struct Tag {
    std::string name;
    uint16 type;
    std::vector<uint8> mask;
  };

  std::shared_ptr<NGDP::Encoding> encoding_;
  std::vector<uint32> file_sizes_;
  std::vector<Tag> tags_;

  static std::vector<Tag> loadTags(File& file, uint32 numTags, uint32 numEntries);
  std::vector<uint8> downloadMask(size_t numFiles);

  // step 4

public:
  struct Progress {
    std::vector<std::string> log;

    uint32 filesTotal = 0;
    uint32 filesDone = 0;
    uint64 sizeTotal = 0;
    uint64 sizeDone = 0;
  };

  void download(std::string const& path);
  std::shared_ptr<Progress> progress() {
    return progress_;
  }
  void stopDownload() {
    stop();
    progress_.reset();
  }

private:
  std::shared_ptr<Progress> progress_;

  // internal stuff

public:
  ProgramData(Application* app)
    : app_(app)
  {}

private:
  Application* app_;
  class Task {
  public:
    virtual ~Task();

    void start(ProgramData* data);
    void stop();

  protected:
    virtual uint32 run() { return 0; };
    void notify(uint32 value = 0);
    ProgramData* data_ = nullptr;
    bool terminate_ = false;

    File loadData(std::string const& hash, int idx = 0);
    File loadData(const NGDP::Hash hash, int idx = 0) {
      return loadData(NGDP::to_string(hash), idx);
    }

  private:
    int proc();
    HANDLE thread_ = nullptr;
    Page* callback_ = nullptr;
    uint32 lastNotify_ = 0;
    uint32 lastNotifyCode_ = 0;
  };
  friend class Task;
  std::shared_ptr<Task> task_;
  void setTask(Task* task);
};
