#pragma once
#include "base/common.h"
#include "base/json.h"
#include "base/file.h"
#include <unordered_map>

namespace NGDP {

  extern const std::string HOST;

  extern const std::map<std::string, std::string> ProgramCodes;

  typedef uint8 Hash[16];
  void from_string(Hash hash, std::string const& str);
  std::string to_string(const Hash hash);

  struct Hash_container {
    Hash _;
    struct hash {
      size_t operator()(Hash_container const& hash) const {
        return *reinterpret_cast<size_t const*>(hash._);
      }
    };
    struct equal {
      bool operator()(Hash_container const& lhs, Hash_container const& rhs) const {
        return !memcmp(lhs._, rhs._, sizeof(Hash));
      }
    };
    static Hash_container const& from(const Hash hash) {
      return *reinterpret_cast<Hash_container const*>(hash);
    }
  };

  struct CdnData {
    std::string path;
    std::vector<std::string> hosts;
  };
  struct VersionData {
    std::string build;
    std::string cdn;
    uint32 id;
    std::string version;
  };

  class NGDP {
  public:
    NGDP(std::string const& app);

    std::string const& program() const {
      return program_;
    }
    std::string const& region() const {
      return region_;
    }

    std::vector<std::string> regions() const {
      return regions_;
    }
    bool setRegion(std::string const& region);

    VersionData const* version() const {
      return getptr(versions_, region_);
    }

    CdnData const* cdn() const {
      return getptr(cdns_, region_);
    }

    std::string geturl(std::string const& hash, std::string const& type = "config", bool index = false) const;
    File load(std::string const& hash, std::string const& type = "config", bool index = false) const;
    File load(const Hash hash, std::string const& type = "config", bool index = false) const {
      return load(to_string(hash), type, index);
    }

  private:
    std::string program_;
    std::string region_;
    std::map<std::string, CdnData> cdns_;
    std::map<std::string, VersionData> versions_;
    std::vector<std::string> regions_;
    std::string base_;
  };

  File DecodeBLTE(File& blte, uint32 usize = 0);
  typedef std::map<std::string, std::string> ConfigFile;
  ConfigFile ParseConfig(File& file);

  class Encoding {
  public:
    Encoding(File& file);

#pragma pack(push, 1)
    struct EncodingEntry {
      uint16 keyCount;
      uint32 usize;
      Hash hash;
      Hash keys[1];
    };
    struct LayoutEntry {
      Hash key;
      uint32 stringIndex;
      uint8 unk;
      uint32 csize;
    };
#pragma pack(pop)

    EncodingEntry const* getEncoding(const Hash hash) const;
    LayoutEntry const* getLayout(const Hash key) const;

    char const* const& layout(uint32 index) const {
      return layouts_[index];
    }
    char const* const& layout() const {
      return layout_;
    }

  private:
    std::vector<uint8> data_;
    struct EncodingHeader {
      Hash hash;
      std::vector<EncodingEntry*> entries;
    };
    struct LayoutHeader {
      Hash key;
      std::vector<LayoutEntry*> entries;
    };
    std::vector<EncodingHeader> encodingTable_;
    std::vector<LayoutHeader> layoutTable_;
    std::vector<char*> layouts_;
    char* layout_;
  };

  class CascStorage {
  public:
    CascStorage(std::string const& root);

    File& addConfig(std::string const& hash, File& file);
    File& addIndex(std::string const& hash, File& file);
    File addData(std::string const& name);

    static File& addCache(std::string const& name, File& file);
    static File addCache(std::string const& name);
    static File getCache(std::string const& name);

  private:
    std::string root_;
  };

  class DataStorage {
  public:
    DataStorage(CascStorage& storage);
    ~DataStorage() {
      finish();
    }

    File& addFile(const Hash hash, File& file); // <- original (compressed) file

    void finish() {
      writeIndex();
    }

  private:
    enum {
      MaxIndexEntries = (0x8E000 - 0x28) / 18,
      MaxDataSize = 0x40000000,
    };
    CascStorage& storage_;
    struct IndexEntry {
      Hash hash;
      uint32 size;
      uint16 index;
      uint32 offset;
    };
    std::vector<IndexEntry> index_;
    File data_;
    uint32 indexCount_;
    uint32 dataCount_;

    void writeIndex();
  };

}
