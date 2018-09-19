/*
  pythia_mod82.cxx
*/
#include "Pythia8/Pythia.h"

using namespace Pythia8;

#include <map>

// Functions for histogramming
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/CDFMidPointPlugin.hh"
#include "fastjet/CDFJetCluPlugin.hh"
//#include "fastjet/D0RunIIConePlugin.hh"
#include "TFile.h"
//#include "HepMCInterface.h"
#include "Pythia8Plugins/HepMC2.h"
#include "HepMC/GenEvent.h"   
#include "HepMC/IO_GenEvent.h"

#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TApplication.h"
#include "ExRootAnalysis/ExRootTreeWriter.h"
#include "ExRootAnalysis/ExRootClasses.h"
#include "ExPythia/MyMergingHooks.hxx"
#include "McData/FastjetTools.hxx"
#include "McData/HepmcTools.hxx"
#include "McData/McParticle.hxx"
#include "McData/McVertex.hxx"
#include "McData/Jet.hxx"
#include "McData/IndexPair.hxx"

int main(int argc, char* argv[]) {
  // Check that correct number of command-line arguments
  if (argc != 4) {
    cerr << " Unexpected number of command-line arguments. \n You are"
         << " expected to provide the arguments \n"
         << " 1. Input file for settings \n"
         << " 2. Full name of the input LHE file (with path) \n"
         << " 3. Path for output ROOT files \n"
         << " Program stopped. " << endl;
    return 1;
  }

  Pythia pythia;

  // Input parameters:
  //  1. Input file for settings
  //  2. Path to input LHE file
  //  3. Output histogram path
  pythia.readFile(argv[1]);
  string iPath = string(argv[2]);
  string oPath = string(argv[3]);

  // Number of events
  int nEvent = pythia.mode("Main:numberOfEvents");

  // Construct user inut for merging
  MergingHooks* myMergingHooks = new MyMergingHooks();
  pythia.setMergingHooksPtr( myMergingHooks );

  // For ISR regularisation off
  pythia.settings.forceParm("SpaceShower:pT0Ref",0.);

  // // Declare histograms
  // Hist histPTFirst("pT of first jet",100,0.,100.);
  // Hist histPTSecond("pT of second jet",100,0.,100.);

  // Read in ME configurations
  pythia.readString("Beams:frameType = 4");
  pythia.readString("Beams:LHEF = " + iPath);
  pythia.init();

  // Interface for conversion from Pythia8::Event to HepMC event. 
  std::string hepmc_out="a.hepmc";
  //  HepMC::I_Pythia8 ToHepMC;
  HepMC::Pythia8ToHepMC ToHepMC;
  // Specify file where HepMC events will be stored.
  //  HepMC::IO_GenEvent ascii_io(hepmc_out, std::ios::out);

  //---------------------------------------------------------------
  // Prepare ROOT output
  //---------------------------------------------------------------
  gROOT->SetBatch();
  char appName[] = "pythia_mod2.exe";
  int appargc = 1;
  char *appargv[] = {appName};
  TApplication app(appName, &appargc, appargv);

  TFile* fout = TFile::Open(oPath.c_str(), "RECREATE");

  TClonesArray* ca_particles = new TClonesArray("McParticle", 10000);
  TClonesArray* ca_vertices = new TClonesArray("McVertex", 10000);
  TClonesArray* ca_jets = new TClonesArray("Jet", 1000);
  TClonesArray* ca_jet_matching = new TClonesArray("IndexPair", 10000);
  std::map<HepMC::GenVertex*, int> vtx_id_map;

  //---------------------------------------------------------------
  TTree* t = new TTree("t", "TTree for MC particles ans jets");

  t->Branch("Particle", "TClonesArray", &ca_particles);
  t->Branch("Vertex", "TClonesArray", &ca_vertices);
  t->Branch("Jet", "TClonesArray", &ca_jets);
  t->Branch("Jet_match", "TClonesArray", &ca_jet_matching);

  const fastjet::JetDefinition& jet_def = getJetDefinition("");
  std::vector<fastjet::PseudoJet>::const_iterator pj;
  std::vector<fastjet::PseudoJet> jets;
  //---------------------------------------------------------------

  // Start generation loop
  for( int iEvent=0; iEvent<nEvent; ++iEvent ){

    // Generate next event
    if( ! pythia.next()) continue;

    // Get CKKWL weight of current event
    double weight = pythia.info.mergingWeight();

    // // Fill bins with CKKWL weight
    // double pTfirst = pTfirstJet(pythia.event,1, 0.4);
    // double pTsecnd = pTfirstJet(pythia.event,2, 0.4);
    // histPTFirst.fill( pTfirst, weight);
    // histPTSecond.fill( pTsecnd, weight);

    HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
    ToHepMC.fill_next_event( pythia, hepmcevt );

    // Run jet algorithm
    t->Clear();
    ca_particles->Clear();
    ca_vertices->Clear();
    ca_jets->Clear();
    ca_jet_matching->Clear();

    fastjet::ClusterSequence* cs = runJetAlgorithm(*hepmcevt, jet_def, jets);

    convertVertexData(*hepmcevt, ca_vertices, vtx_id_map);
    convertParticleData(*hepmcevt, ca_particles, vtx_id_map);
    convertJetData(jets, ca_jets, ca_jet_matching);
    
    delete cs;
    t->Fill();

    // Write the HepMC event to file. Done with it.
    //    ascii_io << hepmcevt;

    delete hepmcevt;

    if(iEvent%1000 == 0) cout << iEvent << endl;

  } // end loop over events to generate

  // print cross section, errors
  pythia.stat();

  t->Write("t");

  fout->Write();
  fout->Close();
  delete fout;

  delete myMergingHooks;
  
  return 0;
}
