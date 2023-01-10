#ifndef __TrackExtrapolator_hxx__
#define __TrackExtrapolator_hxx__
/*
  TrackExtrapolator.hxx
*/

class TrackExtrapolator {
public:
  TrackExtrapolator();
  ~TrackExtrapolator();

  double distance(const Hit& hit);

  Point pointClosestTo(const Hit& hit);

  Point pointAt(double s);

  std::vector<double> derivativesAt(double s);
  
protected:
  Track mTrack;
  double mPathLength;
  std::vector<double> mDerivatives;
  const Hit* mHit;
};

#endif // __TrackExtrapolator_hxx__
