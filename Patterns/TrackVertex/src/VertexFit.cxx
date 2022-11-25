/*
  VertexFit.cxx
*/
#include <cmath>
#include "TrackVertex/VertexFit.hxx"
#include "TrackVertex/Vertex.hxx"

VertexFit::VertexFit() {
}

VertexFit::~VertexFit() {
}

double VertexFit::operator()(const double* p) {
  return calculateChi2(p);
}

double VertexFit::calculateChi2(const double* p) {
  double vx = p[0];
  double vy = p[1];
  Vertex v(vx, vy);
  double deriv[3];
  unsigned int i, j;
  double l=0.0;
  double dl = 0.0;
  double Vl = 0.0;
  const double* cov = nullptr;
  
  mChi2 = 0.0;
  for (auto track: mTracks) {
    for (i=0; i<3; ++i) {
      deriv[i] = calculateDerivative(i, v, *track);
    }
    cov = track->covarianceMatrix();
    l = track->distance(v);
    Vl = deriv[0]*deriv[0]*cov[0]
      + deriv[0]*deriv[1]*cov[1]
      + deriv[0]*deriv[2]*cov[2]
      + deriv[1]*deriv[0]*cov[3]
      + deriv[1]*deriv[1]*cov[4]
      + deriv[1]*deriv[2]*cov[5]
      + deriv[2]*deriv[0]*cov[6]
      + deriv[2]*deriv[1]*cov[7]
      + deriv[2]*deriv[2]*cov[8];
    mChi2 += l*l/Vl;
  }
  return mChi2;  
}
  
int VertexFit::fitTracks(const std::vector<Track*>& tracks,
			 const Vertex& vertex) {
  return 0;
}

double VertexFit::calculateDerivative(unsigned int ipar,
				      const Vertex& vertex,
				      const Track& track) {
  double dx = 1.0E-6;
  double dl1 = 0.0;
  double dl2 = 0.0;
  double deriv = 0.0;
  
  Track track2(track);
  track2.setParameter(ipar, track.parameter(ipar)+dx);
  
  dl1 = track.distance(vertex);
  dl2 = track2.distance(vertex);
  deriv = (dl2 - dl1)/dx;

  return deriv;
}

