/*
  SvgIO.cxx
*/
#include <iostream>
#include "TMath.h"

#include "TrackVertex/SvgIO.hxx"

SvgIO::SvgIO() : mOutputFile() {
}

SvgIO::~SvgIO() {
}

int SvgIO::openOutput(const std::string& filename) {
  mOutputFile.open(filename.c_str());
  return 0;
}

int SvgIO::outputEvent(const Event& event, const std::string& prefix) {
  mOutputFile << prefix
	      << "<svg xmlns=\"http://www.w3.org/2000/svg\" \n"
	      << "  width=\"600\" height=\"600\""
	      << "  viewBox=\"-2000 -2000 4000 4000\">"
	      << std::endl;
  
  for (auto track: event.tracks()) {
    outputTrack(*track, prefix+"  ");
  }
  for (auto hit: event.hits()) {
    outputHit(*hit, prefix+"  ");
  }
  
  mOutputFile << prefix
	      << "</svg>"
	      << std::endl;
  return 0;
}

int SvgIO::outputTrack(const Track& track, const std::string& prefix) {
  char line[200];
  double y0 = 1000.0;
  
  double s = 1500.0;
  auto p1 = track.x0();
  auto p2 = track.pointAt(s);

  float x1 = p1.x();
  float y1 = -p1.y();
  float r = track.radius();
  float angle = 0.0;
  int farpoint = 0;
  int clockwize = track.clockwize();
  float x2 = p2.x();
  float y2 = -p2.y();

  float pi = TMath::Pi();
  // std::cout << "p[0]: " << track.parameter(0) << std::endl;
  // std::cout << "p[1]: " << track.parameter(1) << std::endl;
  // std::cout << "p[2]: " << track.parameter(2)*180.0/pi << std::endl;

  std::sprintf(line,
	       "<path d=\"M %5.3f %5.3f A %5.3f %5.3f %5.3f %d %d %5.3f %5.3f\"",
	       x1, y1, r, r, angle, farpoint, clockwize, x2, y2);
  std::sprintf(line, "%s stroke=\"black\" stroke-width=\"2\"", line);
  std::sprintf(line, "%s fill=\"none\" />", line);

  mOutputFile << prefix << line << std::endl;

  return 0;
}

int SvgIO::outputHit(const Hit& hit, const std::string& prefix) {
  char line[200];
  float x = hit.position().x();
  float y = -hit.position().y();
  float r = 5.0;
  
  std::sprintf(line,
	       "<circle cx=\"%5.3f\" cy=\"%5.3f\" r=\"%5.3f\"",
	       x, y, r);
  std::sprintf(line, "%s stroke=\"none\" fill=\"red\" />", line);
  mOutputFile << prefix << line << std::endl;
  
  return 0;
}

int SvgIO::closeOutput() {
  mOutputFile.close();
  return 0;
}

