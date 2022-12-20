#ifndef __SvgIO_hxx__
#define __SvgIO_hxx__
/*
  SvgIO.hxx
*/
#include <fstream>
#include <vector>
#include <tuple>
#include "TrackVertex/Event.hxx"
#include "TrackVertex/Track.hxx"
#include "TrackVertex/Hit.hxx"

class SvgIO {
public:
  typedef std::tuple<int, double, double, double> Spread_t;
  
public:
  SvgIO();
  ~SvgIO();

  int openOutput(const std::string& filename);
  int outputEvent(const Event& event, const std::string& prefix="");
  int outputTrack(const Track& track, const std::string& prefix="");
  int outputHit(const Hit& hit, const std::string& prefix="");
  int closeOutput();

  std::vector<Spread_t> trackUncertainties(const Track& track);

  Spread_t updateSpread(const double* pars, const Spread_t& spread);

  Spread_t updateSpread1(const Spread_t& spread,
			 const Spread_t& spread_current);
  
protected:
  std::ofstream mOutputFile;
};

#endif // __SvgIO_hxx__
