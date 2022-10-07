#ifndef __SimpleDetector_hxx__
#define __SimpleDetector_hxx__
/*
  SimpleDetector.hxx
*/
#include <cstdint>
#include <vector>
#include "TNamed.h"

#include "TrackVertex/Point.hxx"
#include "TrackVertex/Track.hxx"
#include "TrackVertex/Hit.hxx"

class SimpleDetector : public TNamed {
public:
  SimpleDetector();
  SimpleDetector(std::uint32_t nlayers, float rmin, float rmax);
  ~SimpleDetector();

  std::uint32_t nLayers() const { return mNLayers; }

  float rMin() const { return mRMin; }
  float rMax() const { return mRMax; }

  float radiusOfLayer(std::uint32_t layer) const;

  bool intersectionAtLayer(const Track& track, std::uint32_t layer,
			   Point& point) const;
  Hit smearHitAtLayer(const Hit& hit, std::uint32_t layer, float resolution);
  
  ClassDef(SimpleDetector, 1)
  
protected:
  std::uint32_t mNLayers;
  float mRMin;
  float mRMax;
  std::vector<float> mRadiusList;
};

#endif // __SimpleDetector_hxx__
