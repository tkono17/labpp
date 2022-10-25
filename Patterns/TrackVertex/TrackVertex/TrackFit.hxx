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

  double chi2() const { return mChi2; }
  
  const double* parameters() const { return mParameters; }
  
protected:
  std::vector<Hit*> mHits;
  Track mTrack;
  double mChi2;
  double mParameters[3];
  
};

#endif // __TrackFit_hxx__
