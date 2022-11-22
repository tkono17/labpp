#ifndef __VertexFit_hxx__
#define __VertexFit_hxx__
/*
  VertexFit.hxx
*/
#include <vector>
#include "TrackVertex/Track.hxx"
#include "TrackVertex/Vertex.hxx"

class VertexFit {
public:
  VertexFit();
  ~VertexFit();

  double chi2() const { return mChi2; }
  const double* parameters() const { return mParameters; }

  double operator()(const double* p);

  double calculateChi2(const double* p);
  
  int fitTracks(const std::vector<Track*>& tracks, const Vertex& vertex);

  double calculateDerivative(unsigned int ipar,
			     const Vertex& vertex,
			     const Track& track);

protected:
  double mChi2;
  double mParameters[2];
  
  std::vector<Track*> mTracks;
};

#endif // __VertexFit_hxx__
