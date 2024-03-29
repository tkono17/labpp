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
  char line[2000];
  double y0 = 1000.0;

  double smax = 1500.0;
  auto p1 = track.x0();
  auto p2 = track.pointAt(smax);

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

  double ds = 100.0;
  double s = 0.0;
  int i_s = 0;
  std::vector<Spread_t> vout = trackUncertainties(track);
  for (auto& spread: vout) {
    i_s = std::get<0>(spread);
    s = std::get<1>(spread);
    auto d1 = std::get<2>(spread);
    auto d2 = std::get<3>(spread);
    
    std::cout << "Track uncertainty: s=" << s << ", d1=" << d1
	      << ", d2=" << d2 << std::endl;
    auto p0 = track.pointAt(s);
    auto dir0 = track.directionAt(s);
    auto dp1 = p0 - d1*dir0;
    auto dp2 = p0 + d2*dir0;
    std::cout << "s=" << s << " d1=" << d1 << ", d2=" << d2 << std::endl;
  }
  
  std::sprintf(line,
	       "<path d=\"M %5.3f %5.3f A %5.3f %5.3f %5.3f %d %d %5.3f %5.3f\"",
	       x1, y1, r, r, angle, farpoint, clockwize, x2, y2);
  std::sprintf(line, "%s stroke=\"black\" stroke-width=\"2\"", line);
  std::sprintf(line, "%s fill=\"none\" />", line);

  mOutputFile << prefix << line << std::endl;

  return 0;
}

int SvgIO::outputHit(const Hit& hit, const std::string& prefix) {
  char line[2000];
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

std::vector<SvgIO::Spread_t>
SvgIO::trackUncertainties(const Track& track) {
  std::vector<Spread_t> vout;
  
  const double* covArray = track.covarianceMatrix();
  double cov[3][3];
  int i, j, k;
  double smax = 1500.0;
  double ds = 100.0;
  double s = 0.0;
  
  if (covArray) {
    for (i=0; i<3; ++i) {
      for (j=0; j<3; ++j) {
	k = 3*i + j;
	cov[i][j] = covArray[k];
      }
    }
  }

  Track track1;
  Track track2;
  double d1=0.0, d2=0.0;
  double d=0.0;
  Point p1, p2;
  double pars0[3];
  double pars[3];
  int n = static_cast<int>(smax/ds);

  int parPairs[3][3] = {
			{ 0, 1, 2 }, 
			{ 0, 2, 1 }, 
			{ 1, 2, 0 }
  };
			
  
  Spread_t spread;
  for (i=0; i<n; ++i) {
    s = ds*i;
    auto p0 = track.pointAt(s);

    d1 = 0.0;
    d2 = 0.0;
    spread = { i, s, d1, d2 };
    
    for (k=0; k<3; ++k) {
      for (j=0; j<3; ++j) {
	pars0[j] = track.parameter(j);
	pars[j] = track.parameter(j);
      }
      int i1 = parPairs[k][0];
      int i2 = parPairs[k][1];
      double sigma1 = std::sqrt(cov[i1][i1]);
      double sigma2 = std::sqrt(cov[i2][i2]);
      double r = cov[i1][i2]/(sigma1*sigma2);
      std::cout << "   *** sigma1=" << sigma1
		<< " sigma2=" << sigma2
		<< " r=" << r << std::endl;
      // Variation 1a
      pars[i1] = pars0[i1] + sigma1;
      pars[i2] = pars0[i2] + sigma2*r;
      spread = updateSpread(pars, spread);
      // Variation 1b
      pars[i1] = pars0[i1] - sigma1;
      pars[i2] = pars0[i2] - sigma2*r;
      // Variation 2a
      pars[i1] = pars0[i1] + sigma1*r;
      pars[i2] = pars0[i2] + sigma2;
      // Variation 2b
      pars[i1] = pars0[i1] - sigma1*r;
      pars[i2] = pars0[i2] - sigma2;
      // Variation 3a
      pars[i1] = pars0[i1] + sigma1*std::sqrt(1.0+r);
      pars[i2] = pars0[i2] + sigma2*std::sqrt(1.0+r);
      // Variation 3b
      pars[i1] = pars0[i1] - sigma1*std::sqrt(1.0-r);
      pars[i2] = pars0[i2] - sigma2*std::sqrt(1.0-r);
    }
    vout.push_back(std::make_tuple(i, s, d1, d2));
  }
  return vout;
}

SvgIO::Spread_t
SvgIO::updateSpread(const double* pars, const SvgIO::Spread_t& spread0) {
  auto i_s = std::get<0>(spread0);
  auto s = std::get<1>(spread0);
  auto d1 = std::get<2>(spread0);
  auto d2 = std::get<3>(spread0);

  Track track1(pars[0], pars[1], pars[2]);
  auto p = track1.pointAt(s);
  auto d = track1.distance(Vertex(p.x(), p.y()));
  Spread_t spread1 = { i_s, s, d, d };
  std::cout << " ---> point=" << p.x() << ", " << p.y() << " d=" <<  d << std::endl;
  
  return updateSpread1(spread1, spread0);
}

SvgIO::Spread_t updateSpread1(const SvgIO::Spread_t& spread,
			      const SvgIO::Spread_t& spread_current) {
  auto i_s = std::get<0>(spread_current);
  auto s = std::get<1>(spread_current);
  auto d1 = std::get<2>(spread_current);
  auto d2 = std::get<3>(spread_current);

  if (std::get<2>(spread) > d1) {
    d1 = std::get<2>(spread);
  }
  if (std::get<3>(spread) > d2) {
    d2 = std::get<2>(spread);
  }
  return { i_s, s, d1, d2 };
}


