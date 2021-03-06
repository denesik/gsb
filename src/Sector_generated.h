// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_SECTOR_GSB_FLAT_H_
#define FLATBUFFERS_GENERATED_SECTOR_GSB_FLAT_H_

#include "flatbuffers/flatbuffers.h"

namespace gsb_flat {

struct SPos;

struct Sector;

MANUALLY_ALIGNED_STRUCT(4) SPos FLATBUFFERS_FINAL_CLASS {
 private:
  int32_t x_;
  int32_t z_;

 public:
  SPos() {
    memset(this, 0, sizeof(SPos));
  }
  SPos(const SPos &_o) {
    memcpy(this, &_o, sizeof(SPos));
  }
  SPos(int32_t _x, int32_t _z)
      : x_(flatbuffers::EndianScalar(_x)),
        z_(flatbuffers::EndianScalar(_z)) {
  }
  int32_t x() const {
    return flatbuffers::EndianScalar(x_);
  }
  int32_t z() const {
    return flatbuffers::EndianScalar(z_);
  }
};
STRUCT_END(SPos, 8);

struct Sector FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_MPOS = 4,
    VT_MSTATICBLOCKS = 6
  };
  const SPos *mPos() const {
    return GetStruct<const SPos *>(VT_MPOS);
  }
  const flatbuffers::Vector<int32_t> *mStaticBlocks() const {
    return GetPointer<const flatbuffers::Vector<int32_t> *>(VT_MSTATICBLOCKS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<SPos>(verifier, VT_MPOS) &&
           VerifyOffset(verifier, VT_MSTATICBLOCKS) &&
           verifier.Verify(mStaticBlocks()) &&
           verifier.EndTable();
  }
};

struct SectorBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_mPos(const SPos *mPos) {
    fbb_.AddStruct(Sector::VT_MPOS, mPos);
  }
  void add_mStaticBlocks(flatbuffers::Offset<flatbuffers::Vector<int32_t>> mStaticBlocks) {
    fbb_.AddOffset(Sector::VT_MSTATICBLOCKS, mStaticBlocks);
  }
  SectorBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  SectorBuilder &operator=(const SectorBuilder &);
  flatbuffers::Offset<Sector> Finish() {
    const auto end = fbb_.EndTable(start_, 2);
    auto o = flatbuffers::Offset<Sector>(end);
    return o;
  }
};

inline flatbuffers::Offset<Sector> CreateSector(
    flatbuffers::FlatBufferBuilder &_fbb,
    const SPos *mPos = 0,
    flatbuffers::Offset<flatbuffers::Vector<int32_t>> mStaticBlocks = 0) {
  SectorBuilder builder_(_fbb);
  builder_.add_mStaticBlocks(mStaticBlocks);
  builder_.add_mPos(mPos);
  return builder_.Finish();
}

inline flatbuffers::Offset<Sector> CreateSectorDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const SPos *mPos = 0,
    const std::vector<int32_t> *mStaticBlocks = nullptr) {
  return gsb_flat::CreateSector(
      _fbb,
      mPos,
      mStaticBlocks ? _fbb.CreateVector<int32_t>(*mStaticBlocks) : 0);
}

inline const gsb_flat::Sector *GetSector(const void *buf) {
  return flatbuffers::GetRoot<gsb_flat::Sector>(buf);
}

inline bool VerifySectorBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<gsb_flat::Sector>(nullptr);
}

inline void FinishSectorBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gsb_flat::Sector> root) {
  fbb.Finish(root);
}

}  // namespace gsb_flat

#endif  // FLATBUFFERS_GENERATED_SECTOR_GSB_FLAT_H_
