/*
  TrackFit.cxx
*/
#include <iostream>

#include "Math/Factory.h"
#include "Math/Functor.h"
#include "Math/Minimizer.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/MnUserParameterState.h"

#include "TrackVertex/TrackFit.hxx"

TrackFit::TrackFit() :
  mHits() {
  int i;

  mResolution = 0.05;
  for (i=0; i<3; ++i) {
    mParameters[i] = 0.0;
  }
  for (i=0; i<9; ++i) {
    mCovMatrix[i] = 0.0;
  }
  mChi2 = 0.0;
  mNDof = 0;
  mFitFailed = false;
}

TrackFit::~TrackFit() {
}

double TrackFit::operator()(const std::vector<Double_t>& p) {
  double pars[3];
  for (int i=0; i<3; ++i) {
    pars[i] = p[i];
  }
  double y = calculateChi2(pars);
  return y;
}

double TrackFit::operator()(const double* p) {
  double y = calculateChi2(p);

  //  y = p[0]*p[0] + (p[1]-2.0)*(p[1]-2.0) + p[2]*p[2];
  return y;
}

double TrackFit::calculateChi2(const double* p, bool debug) {
  // p[0]: rho
  // p[1]: d0
  // p[2]: phi0
  double chi2 = 0.0;
  double sigma = mResolution;
  Track track;
  
  track.setParameters(p);

  //  std::cout << "    Track p=" << p[0] << ", " << p[1] << ", " << p[2] << std::endl;
  // std::cout << "Track r=" << track.circleR()
  // 	    << " c=(" << track.circleCenter().x() << ", "
  // 	    << track.circleCenter().y() << ")"
  // 	    << std::endl;
  int i=0;
  for (auto phit: mHits) {
    double d = track.distance(*phit);
    if (debug) {
      std::cout << "      hit[" << i << "] distance=" << d << std::endl;
    }
    i ++;
    d /= sigma;
    chi2 += d*d;
  }
  
  return chi2;
}

int TrackFit::fitHits(const std::vector<Hit*>& hits, const Track& track) {
  return fitHitsMinuit2(hits, track);
  //return fitHitsNum(hits, track);
}

int TrackFit::fitHitsMinuit2(const std::vector<Hit*>& hits,
			     const Track& track) {
  mHits = hits;
  mFitFailed = false;

  double variables[3];
  double steps[3] = { 1.0E-6, 1.0E-4, 1.0E-4};
  
  variables[0] = track.parameter(0);
  variables[1] = track.parameter(1);
  variables[2] = track.parameter(2);

  //  ROOT::Math::Functor f(*this, 3);
  FitFCN fitfunc;
  fitfunc.setTrackFit(this);

  ROOT::Minuit2::MnUserParameters upar;
  // 引数は(変数名, 初期値, ステップ幅, 最小値, 最大値)
  upar.Add("rho", variables[0], steps[0], -10.0, 10.0);
  upar.Add("d0", variables[1], steps[1], -10.0, 10.0);
  upar.Add("phi0", variables[2], steps[2], -100.0, 100.0);
  ROOT::Minuit2::MnMigrad migrad(fitfunc, upar);
  ROOT::Minuit2::FunctionMinimum min = migrad();
  ROOT::Minuit2::MnUserParameterState state = min.UserState();
  std::cout << "User state: " << state << std::endl;

  for (int i=0; i<3; ++i) {
    mParameters[i] = state.Value(i);
  }
  mChi2 = state.Fval();
  mNDof = static_cast<int>(hits.size()) - 3;
  return 0;
}

int TrackFit::fitHitsNum(const std::vector<Hit*>& hits, const Track& track) {
  mHits = hits;
  mFitFailed = false;

  int i, j;
  int ncallsMax = 1000000;
  double parsInit[3];
  double pars[3];
  double steps[3] = { 1.0E-6, 1.0E-4, 1.0E-4};
  double derivatives[3];
  
  for (i=0; i<3; ++i) {
    parsInit[i] = track.parameter(i);
  }
  // Calculate derivatives
  for (i=0; i<3; ++i) {
    pars[i] = parsInit[i];
  }
  double y0 = calculateChi2(pars);
  
  pars[0] += steps[0];
  double y1 = calculateChi2(pars);

  for (i=0; i<3; ++i) {
    pars[i] = parsInit[i];
  }
  pars[1] += steps[1];
  double y2 = calculateChi2(pars);

  for (i=0; i<3; ++i) {
    pars[i] = parsInit[i];
  }
  pars[2] += steps[2];
  double y3 = calculateChi2(pars);
  derivatives[0] = (y1 - y0)/steps[0];
  derivatives[1] = (y2 - y0)/steps[1];
  derivatives[2] = (y3 - y0)/steps[2];

  y0 = 0.0;
  std::cout << "dy = " << (y1-y0) << ", " << (y2-y0) << ", " << (y3-y0)
	    << std::endl;
  // Find the minimum
  
  return 0;
}

int TrackFit::fitHitsMinimizer(const std::vector<Hit*>& hits, const Track& track) {
  mHits = hits;
  mFitFailed = false;
  
  std::cout << "Debug before fitting" << std::endl;
  calculateChi2(track.parameters(), true);
  
  std::string minName="Minuit2";
  std::string minAlgo = "Migrad";
  // std::string minName="GSLMultiMin";
  // std::string minAlgo = "ConjugateFR";
  int i, j, k;
  
  ROOT::Math::Minimizer* minimizer =
    ROOT::Math::Factory::CreateMinimizer(minName, minAlgo);

  minimizer->SetMaxFunctionCalls(1000000);
  minimizer->SetMaxIterations(10000);
  minimizer->SetTolerance(0.001);
  minimizer->SetPrintLevel(1);

  ROOT::Math::Functor f(*this, 3);
  double steps[3] = { 1.0E-6, 1.0E-4, 1.0E-4};
  double variables[3];
  std::string varNames[3];
  varNames[0] = "q/R";
  varNames[1] = "d0";
  varNames[2] = "phi0";

  variables[0] = track.parameter(0);
  variables[1] = track.parameter(1);
  variables[2] = track.parameter(2);  

  std::cout << "Track initial parameters" << std::endl;
  for (i=0; i<3; ++i) {
    std::cout << "  p[" << i << "] = " << variables[i] << std::endl;
  }

  minimizer->SetFunction(f);
  for (i=0; i<3; ++i) {
    minimizer->SetVariable(i, varNames[i].c_str(), variables[i], steps[i]);
  }
  minimizer->Minimize();

  const double* xs = minimizer->X();

  std::cout << "   ***** Minimization: chi2=" << minimizer->MinValue()
	    <<", Ndim=" << minimizer->NDim()
	    << std::endl;
  mChi2 = minimizer->MinValue();
  mNDof = static_cast<int>(mHits.size()) - 3;
  for (i=0; i<3; ++i) {
    std::cout << "  fit values: p[" << i << "] = " << xs[i] << std::endl;
    mParameters[i] = xs[i];
  }
  for (i=0; i<3; ++i) {
    for (j=0; j<3; ++j) {
      k = 3*i + j;
      mCovMatrix[k] = minimizer->CovMatrix(i, j);
    }
  }
  if (minimizer->IsValidError()) {
    mFitFailed = false;
  } else {
    mFitFailed = true;
  }
  std::cout << "Fit status " << minimizer->Status() << " failed?=" << mFitFailed << ", Edm=" << minimizer->Edm() << std::endl;
  minimizer->PrintResults();

  return 0;
}

int TrackFit::estimatePars(const std::vector<Hit*>& hits, double* pars) {
  int i=0;
  for (i=0; i<3; ++i) {
    pars[i] = 0.0;
  }
  
  if (hits.size() < 3) return -1;
  
  auto& p1 = hits[0]->position();
  auto& p2 = hits[1]->position();
  auto& p3 = hits[2]->position();

  double x1 = p1.x();
  double y1 = p1.y();
  double x2 = p2.x();
  double y2 = p2.y();
  double x3 = p3.x();
  double y3 = p3.y();

  double a = 2.0*(x1 - x2);
  double b = 2.0*(y1 - y2);
  double c = 2.0*(x1 - x3);
  double d = 2.0*(y1 - y3);
  double u = x1*x1 + y1*y1 - x2*x2 - y2*y2;
  double v = x1*x1 + y1*y1 - x3*x3 - y3*y3;
  
  double D = a*d - b*c;
  double p = d/D;
  double q = -b/D;
  double r = -c/D;
  double s = a/D;

  double cx = p*u + q*v;
  double cy = r*u + s*v;

  double dx = cx - x1;
  double dy = cy - y1;
  double R = std::sqrt(dx*dx + dy*dy);
  double z = cx*y3 - cy*x3;
  bool clockwize = (z > 0.0);
  double dR = R - std::sqrt(cx*cx + cy*cy);
  
  double rho = 0.0;
  double d0 = 0.0;
  double phi0 = 0.0;
  if (clockwize && dR >= 0.0) {
    rho = 1.0/R;
    d0 = -dR;
  } else if (clockwize && dR < 0.0) {
    rho = 1.0/R;
    d0 = dR;
  } else if (!clockwize && dR >= 0.0) {
    rho = -1.0/R;
    d0 = dR;
  } else if (!clockwize && dR < 0.0) {
    rho = -1.0/R;
    d0 = -dR;
  }
  pars[0] = rho;
  pars[1] = d0;
  pars[2] = phi0;
  return 0;
}

FitFCN::FitFCN() : ROOT::Minuit2::FCNBase() {
  mFitter = nullptr;
  mErrorDef = 0.0;
}

FitFCN::~FitFCN() {
}

Double_t FitFCN::operator()(const std::vector<Double_t>& p) const {
  Double_t y = 0.0;
  if (mFitter) {
    y = (*mFitter)(p);
  }
  return y;
}

