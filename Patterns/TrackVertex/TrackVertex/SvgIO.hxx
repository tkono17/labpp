#ifndef __SvgIO_hxx__
#define __SvgIO_hxx__
/*
  SvgIO.hxx
*/
#include <fstream>
#include <vector>
#include <map>
#include "TrackVertex/Event.hxx"
#include "TrackVertex/Track.hxx"
#include "TrackVertex/Hit.hxx"

class SvgIO {
public:
  SvgIO();
  ~SvgIO();

  int openOutput(const std::string& filename);
  int outputEvent(const Event& event, const std::string& prefix="");
  int outputTrack(const Track& track, const std::string& prefix="");
  int outputHit(const Hit& hit, const std::string& prefix="");
  int closeOutput();

  std::vector<std::pair<double, double> > trackUncertainties(const Track& track);
  
protected:
  std::ofstream mOutputFile;
};

#endif // __SvgIO_hxx__