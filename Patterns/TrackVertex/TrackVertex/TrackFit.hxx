#ifndef __TrackFit_hxx__
#define __TrackFit_hxx__
/*
  TrackFit.hxx
*/
#include <string>
#include <vector>

#include "Minuit2/FCNBase.h"

#include "TrackVertex/Hit.hxx"
#include "TrackVertex/Track.hxx"

class TrackFit {
public:
  TrackFit();
  ~TrackFit();

  void setResolution(double x) { mResolution = x; }
  double resolution() const { return mResolution; }
  
  double operator()(const std::vector<Double_t>& p);
  double operator()(const double* p);

  double calculateChi2(const double* p, bool debug=false);
  
  int fitHits(const std::vector<Hit*>& hits, const Track& track);
  int fitHitsNum(const std::vector<Hit*>& hits, const Track& track);
  int fitHitsMinimizer(const std::vector<Hit*>& hits, const Track& track);
  int fitHitsMinuit2(const std::vector<Hit*>& hits, const Track& track);

  double chi2() const { return mChi2; }
  int nDof() const { return mNDof; }
  
  const double* parameters() const { return mParameters; }
  const double* covMatrix() const { return mCovMatrix; }

  int estimatePars(const std::vector<Hit*>& hits, double* pars);
  
  bool fitFailed() const { return mFitFailed; }
  
protected:
  double mResolution;
  std::vector<Hit*> mHits;
  double mChi2;
  int mNDof;
  double mParameters[3];
  double mCovMatrix[9];
  bool mFitFailed;
};

class FitFCN : public ROOT::Minuit2::FCNBase {
public:
  FitFCN();
  ~FitFCN();

  Double_t operator()(const std::vector<Double_t>& p) const;

  Double_t Up() const { return mErrorDef; }
  
  void SetErrorDef(Double_t x) { mErrorDef = x; }
  
  void setTrackFit(TrackFit* fit) { mFitter = fit; }

protected:
  mutable TrackFit* mFitter;
  Double_t mErrorDef;
};


#endif // __TrackFit_hxx__
