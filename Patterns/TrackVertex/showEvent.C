/*
  showEvent.C
*/
#include <iostream>
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"

#include "TrackVertex/EventDisplay.hxx"
#include "TrackVertex/Event.hxx"
#include "TrackVertex/SimpleDetector.hxx"

void showEvent(const std::string& fname, int ievent) {
  TFile* fin = TFile::Open(fname.c_str(), "READ");
  TTree* t = dynamic_cast<TTree*>(fin->Get("t"));
  Event* event = new Event();
  t->SetBranchAddress("event", &event);
  SimpleDetector* det = dynamic_cast<SimpleDetector*>(fin->Get("detector"));
  
  EventDisplay display(2.0E+3, 2.0E+3);
  
  if (t) {
    TCanvas* c = new TCanvas("c", "", 0, 0, 500, 500);
    display.setPad(c);

    if (ievent < t->GetEntries()) {
      t->GetEntry(ievent);
      display.drawEvent(*event, *det);
    } else {
      std::cout << "TTree has " << t->GetEntries() << " events" << std::endl;
    }
    
  }
}
