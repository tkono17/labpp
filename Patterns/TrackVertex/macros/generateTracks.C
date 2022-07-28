/*
  generateTracks.C
*/
#include <iostream>
#include <cstdint>

#include "TCanvas.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TTree.h"
#include "TFile.h"

#include "TrackVertex/Track.hxx"
#include "TrackVertex/Point.hxx"
#include "TrackVertex/Event.hxx"
#include "TrackVertex/SimpleDetector.hxx"
#include "TrackVertex/EventDisplay.hxx"

std::uint32_t nlayers = 10;
float rmin = 50.0;
float rmax = 1000.0;
std::string outFileName = "tracks.root";
float nMean = 20.0;
float nSigma = 7.0;
float nEvents = 100;

void generateTracks() {
  Point origin(0.0, 0.0);
  Point genPoint(0.05, 0.0);
  float p = 10.0;
  float phi = 0.0;
  TRandom3 random;
  const float pi = TMath::Pi();

  TFile* fout = TFile::Open(outFileName.c_str(), "RECREATE");
  Event* event = new Event();
  TTree* t = new TTree("t", "");
  t->Branch("event", "Event", &event);

  event->setGenerationPoint(genPoint);
  SimpleDetector* det = new SimpleDetector(nlayers, rmin, rmax);
  det->Write();

  Track track;

  float charge = 1.0;

  for (int ievent=0; ievent<nEvents; ++ievent) {
    event->clear();
    
    int n = random.Gaus(nMean, nSigma);
    if (n < 0) n = 3;
    for (int i=0; i<n; ++i) {
      p = random.Uniform(0.1, 10.0);
      phi = random.Uniform(-pi, pi);
      charge = 1.0;
      if (random.Uniform(-1.0, 1.0) < 0.0) charge = -1.0;
      track.setDataPPhiXY(p, phi, genPoint, charge);
      event->addTrack(track);
      auto nlayers = det->nLayers();
      pbox::Point hitp;
      std::vector<Hit*> hits;
      for (std::uint32_t ilayer=0; ilayer<nlayers; ++ilayer) {
	if (det->intersectionAtLayer(track, ilayer, p)) {
	  hits.push_back(new Hit(hitp, 0));
	}
      }
      event->addHitsOnTrack(track, hits);
    }
    t->Fill();
  }
  
  t->Write();
  fout->Write();

  TCanvas* c = new TCanvas("c", "", 0, 0, 500, 500);
  EventDisplay display(2.0E+3, 2.0E+3);
  display.setPad(c);
  display.drawEvent(*event);
}
