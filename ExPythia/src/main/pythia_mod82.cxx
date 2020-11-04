/*
  pythia_mod82.cxx
*/
#include "Pythia8/Pythia.h"

using namespace Pythia8;

// Functions for histogramming
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/CDFMidPointPlugin.hh"
#include "fastjet/CDFJetCluPlugin.hh"
//#include "fastjet/D0RunIIConePlugin.hh"
#include "TFile.h"
#include "Pythia8Plugins/HepMC3.h"
#include "HepMC3/GenEvent.h"   

#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TApplication.h"
#include "ExRootAnalysis/ExRootTreeWriter.h"
#include "ExRootAnalysis/ExRootClasses.h"
#include "McData/FastjetTools.hxx"
#include "McData/HepmcTools.hxx"
#include "McData/Jet.hxx"

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
  HepMC3::Pythia8ToHepMC3 ToHepMC;
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
  //---------------------------------------------------------------
  ExRootTreeWriter *treeWriter = new ExRootTreeWriter(fout, "LHEF");

  ExRootTreeBranch *branchEvent = 
    treeWriter->NewBranch("Event", TRootLHEFEvent::Class());
  ExRootTreeBranch *branchParticle = 
    treeWriter->NewBranch("Particle", TRootLHEFParticle::Class());
  ExRootTreeBranch *branchJet = treeWriter->NewBranch("Jet", Jet::Class());

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

    HepMC3::GenEvent* hepmcevt = new HepMC3::GenEvent();
    ToHepMC.fill_next_event( pythia, hepmcevt );

    // Run jet algorithm
    treeWriter->Clear();
    
    int nparticles = convertParticleData(*hepmcevt, branchParticle);
    convertEventData(*hepmcevt, nparticles, branchEvent);
    
    runJetAlgorithm(*hepmcevt, jet_def, jets);
    convertJetData(jets, branchJet);
    
    treeWriter->Fill();

    // Write the HepMC event to file. Done with it.
    //    ascii_io << hepmcevt;

    delete hepmcevt;

    if(iEvent%1000 == 0) cout << iEvent << endl;

  } // end loop over events to generate

  // print cross section, errors
  pythia.stat();

  treeWriter->Write();

  fout->Write();
  fout->Close();
  delete fout;

  return 0;
}
