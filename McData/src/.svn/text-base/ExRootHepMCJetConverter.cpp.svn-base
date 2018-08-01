/*
  ExRootLHEFJetConverter.cpp
  --------------------------
  Convert LHEF into a ROOT file in a similar way as ExRootAnalysis while
  running the jet algorithm
*/
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "TROOT.h"
#include "TApplication.h"

#include "TFile.h"
#include "TChain.h"
#include "TString.h"

#include "TH2.h"
#include "THStack.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"

#include "ExRootAnalysis/ExRootClasses.h"

#include "ExRootAnalysis/ExRootTreeWriter.h"
#include "ExRootAnalysis/ExRootTreeBranch.h"

#include "McData/HepmcTools.hxx"
#include "McData/FastjetTools.hxx"
#include "McData/Jet.hxx"

using namespace std;


//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  char appName[] = "ExRootLHEFConverter";

  if(argc != 3)
  {
    cout << " Usage: " << appName << " input_file" << " output_file" << endl;
    cout << " input_file - input file in LHEF format," << endl;
    cout << " output_file - output file in ROOT format." << endl;
    return 1;
  }

  gROOT->SetBatch();

  int appargc = 1;
  char *appargv[] = {appName};
  TApplication app(appName, &appargc, appargv);

  std::string fname_in=argv[1];
  std::string fname_out=argv[2];

  std::ifstream fin(fname_in.c_str());

  TFile *outputFile = TFile::Open(argv[2], "RECREATE");
  ExRootTreeWriter *treeWriter = new ExRootTreeWriter(outputFile, "LHEF");

  ExRootTreeBranch *branchEvent = 
    treeWriter->NewBranch("Event", TRootLHEFEvent::Class());
  ExRootTreeBranch *branchParticle = 
    treeWriter->NewBranch("Particle", TRootLHEFParticle::Class());
  ExRootTreeBranch *branchJet = treeWriter->NewBranch("Jet", Jet::Class());

  const fastjet::JetDefinition& jet_def = getJetDefinition("");
  std::vector<fastjet::PseudoJet>::const_iterator pj;
  std::vector<fastjet::PseudoJet> jets;

  int ntotal=0;
  HepMC::GenEvent mcevent;
  while (fin) {


    mcevent.read(fin);
    if (mcevent.is_valid()) {
      ntotal ++;
      
      treeWriter->Clear();
      
      int nparticles = convertParticleData(mcevent, branchParticle);
      convertEventData(mcevent, nparticles, branchEvent);
      
      runJetAlgorithm(mcevent, jet_def, jets);
      convertJetData(jets, branchJet);
      
      treeWriter->Fill();
    }
  }

  treeWriter->Write();

  cout << "Processed " << ntotal << " events" << endl;

  fin.close();

  delete treeWriter;
  delete outputFile;
}

