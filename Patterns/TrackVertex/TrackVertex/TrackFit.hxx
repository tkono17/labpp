#ifndef __TrackFit_hxx__
#define __TrackFit_hxx__
/*
  TrackFit.hxx
*/
#include <string>
#include <vector>

#include "TrackVertex/Hit.hxx"
#include "TrackVertex/Track.hxx"

class TrackFit {
public:
  TrackFit();
  ~TrackFit();

  const Track& track() const { return mTrack; }
  
  double operator()(const double* p);

  double calculateChi2(const double* p);
  
  int fitHits(const std::vector<Hit*>& hits, const Track& track);
  
protected:
  std::vector<Hit*> mHits;
  Track mTrack;
};

#endif // __TrackFit_hxx__
