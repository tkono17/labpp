/*
  TrackFit.cxx
*/
#include <iostream>

#include "Math/Factory.h"
#include "Math/Functor.h"
#include "Math/Minimizer.h"

#include "TrackVertex/TrackFit.hxx"

TrackFit::TrackFit() :
  mHits(), mTrack() {
}

TrackFit::~TrackFit() {
}

double TrackFit::operator()(const double* p) {
  double y = calculateChi2(p);

  //  y = p[0]*p[0] + (p[1]-2.0)*(p[1]-2.0) + p[2]*p[2];
  return y;
}

double TrackFit::calculateChi2(const double* p) {
  // p[0]: rho
  // p[1]: d0
  // p[2]: phi0
  double chi2 = 0.0;
  double sigma = 0.05;
  Track track;
  
  track.setData(p[0], p[1], p[2]);

  std::cout << "Track r=" << track.circleR()
	    << " c=(" << track.circleCenter().x() << ", "
	    << track.circleCenter().y() << ")"
	    << std::endl;
  int i=0;
  for (auto phit: mHits) {
    double d = track.distance(*phit);
    std::cout << "  hit[" << i << "] distance=" << d << std::endl;
    i ++;
    d /= sigma;
    chi2 += d*d;
  }
  
  return chi2;
}
  
int TrackFit::fitHits(const std::vector<Hit*>& hits, const Track& track) {
  mHits = hits;
  mTrack = track;

  std::string minName="Minuit2";
  std::string minAlgo = "";
  
  ROOT::Math::Minimizer* minimizer =
    ROOT::Math::Factory::CreateMinimizer(minName, minAlgo);

  minimizer->SetMaxFunctionCalls(1000000);
  minimizer->SetMaxIterations(10000);
  minimizer->SetTolerance(0.001);
  minimizer->SetPrintLevel(1);

  ROOT::Math::Functor f(*this, 3);
  double steps[3] = { 0.01, 0.01, 0.01};
  double variables[3];
  std::string varNames[3];
  varNames[0] = "q/R";
  varNames[1] = "d0";
  varNames[2] = "phi0";
  
  variables[0] = mTrack.parameter(0);
  variables[1] = mTrack.parameter(1);
  variables[2] = mTrack.parameter(2);

  std::cout << "Track initial parameters" << std::endl;
  for (int i=0; i<3; ++i) {
    std::cout << "  p[" << i << "] = " << variables[i] << std::endl;
  }

  minimizer->SetFunction(f);
  for (int i=0; i<3; ++i) {
    minimizer->SetVariable(i, varNames[i].c_str(), variables[i], steps[i]);
  }
  minimizer->Minimize();

  const double* xs = minimizer->X();

  std::cout << "Minimization: chi=" << minimizer->MinValue() << std::endl;
  mChi2 = minimizer->MinValue();
  for (int i=0; i<3; ++i) {
    std::cout << "  p[" << i << "] = " << xs[i] << std::endl;
    mParameters[i] = xs[i];
  }

  minimizer->PrintResults();

  return 0;
}

