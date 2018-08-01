/*
  ExRootLHEFJetConverter.cpp
  --------------------------
  Convert LHEF into a ROOT file in a similar way as ExRootAnalysis while
  running the jet algorithm
*/
#include <iostream>
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

#include "LHEF.h"

#include "ExRootAnalysis/ExRootClasses.h"

#include "ExRootAnalysis/ExRootTreeWriter.h"
#include "ExRootAnalysis/ExRootTreeBranch.h"

#include "ExRootAnalysis/ExRootUtilities.h"
#include "ExRootAnalysis/ExRootProgressBar.h"

#include "fastjet/ClusterSequence.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"

using namespace std;

//---------------------------------------------------------------------------

void AnalyseEvent(LHEF::Reader *reader, ExRootTreeBranch *branch, Long64_t eventNumber)
{
  const LHEF::HEPEUP &hepeup = reader->hepeup;

  TRootLHEFEvent *element;

  element = (TRootLHEFEvent*) branch->NewEntry();


  element->Number = eventNumber;
  element->Nparticles = hepeup.NUP;
  element->ProcessID = hepeup.IDPRUP;
  element->Weight = hepeup.XWGTUP;
  element->ScalePDF = hepeup.SCALUP;
  element->CouplingQED = hepeup.AQEDUP;
  element->CouplingQCD = hepeup.AQCDUP;
}

//---------------------------------------------------------------------------

void AnalyseParticles(LHEF::Reader *reader, ExRootTreeBranch *branch)
{
  const LHEF::HEPEUP &hepeup = reader->hepeup;

  Int_t particle;
  Double_t signPz, cosTheta;

  TLorentzVector momentum;

  TRootLHEFParticle *element;

  for(particle = 0; particle < hepeup.NUP; ++particle)
  {
    element = (TRootLHEFParticle*) branch->NewEntry();

    element->PID = hepeup.IDUP[particle];
    element->Status = hepeup.ISTUP[particle];
    element->Mother1 = hepeup.MOTHUP[particle].first;
    element->Mother2 = hepeup.MOTHUP[particle].second;
    element->ColorLine1 = hepeup.ICOLUP[particle].first;
    element->ColorLine2 = hepeup.ICOLUP[particle].second;
    element->Px = hepeup.PUP[particle][0];
    element->Py = hepeup.PUP[particle][1];
    element->Pz = hepeup.PUP[particle][2];
    element->E = hepeup.PUP[particle][3];
    element->M = hepeup.PUP[particle][4];

    momentum.SetPxPyPzE(element->Px, element->Py, element->Pz, element->E);

    cosTheta = TMath::Abs(momentum.CosTheta());
    signPz = (momentum.Pz() >= 0.0) ? 1.0 : -1.0;

    element->PT = momentum.Perp();
    element->Phi = momentum.Phi();
    element->Eta = (cosTheta == 1.0 ? signPz*999.9 : momentum.Eta());
    element->Rapidity = (cosTheta == 1.0 ? signPz*999.9 : momentum.Rapidity());

    element->LifeTime = hepeup.VTIMUP[particle];
    element->Spin = hepeup.SPINUP[particle];
  }
}


const fastjet::JetDefinition& getJetDefinition(const std::string& /*algo_type*/) {
  static fastjet::JetDefinition* jet_def(0);
  if (jet_def == 0) {
    // algo_type not used 
    double R=0.7;
    jet_def = new fastjet::JetDefinition(fastjet::antikt_algorithm, R);
  }
  return *jet_def;
}

void runJetAlgorithm(ExRootTreeBranch* branchParticles, 
		     const std::string& treename, 
		     const fastjet::JetDefinition& jet_def, 
		     std::vector<fastjet::PseudoJet>& jets) {
  std::vector<fastjet::PseudoJet> v;
  jets.clear();
  TClonesArray* array(0);
  TTree* t = dynamic_cast<TTree*>(gROOT->FindObject(treename.c_str()));
  if (t) {
    // Retrieve the pointer to the TClonesArray holding particles
    TBranch* br = t->GetBranch("Particle");
    if (br) {
      void** addr = reinterpret_cast<void**>(br->GetAddress());
      if (addr) array = reinterpret_cast<TClonesArray*>(*addr);
    } else {
      cout << "Branch Particle is null" << endl;
    }
  } else {
    cout << "Cannot find TTree with name " << treename << endl;
  }
  if (array) {
    int n = array->GetEntries();
    //    cout << "Number of particles from branch: " << n << endl;
    for (int i=0; i<n; ++i) {
      TRootLHEFParticle* p = 
	dynamic_cast<TRootLHEFParticle*>(array->At(i));
      //      cout << "particle i=" << i << " " << p << endl;
      if (p) {
	int pid2 = abs(p->PID);
	if (p->Status == 1 && pid2 != 12 && pid2 != 14 && pid2 != 16) {
	  v.push_back(fastjet::PseudoJet(p->Px, p->Py, p->Pz, p->E));
	}
      } else {
	cout << "particle is null" << endl;
      }
    }
    fastjet::ClusterSequence cs(v, jet_def);
    jets = sorted_by_pt(cs.inclusive_jets());
  }
}

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

  // Open a stream connected to an event file:
  ifstream inputFileStream(argv[1]);

  // Create the Reader object:
  LHEF::Reader *inputReader = new LHEF::Reader(inputFileStream);

  TFile *outputFile = TFile::Open(argv[2], "RECREATE");
  ExRootTreeWriter *treeWriter = new ExRootTreeWriter(outputFile, "LHEF");

  // generated event from LHEF
  ExRootTreeBranch *branchEvent = treeWriter->NewBranch("Event", TRootLHEFEvent::Class());

  // generated partons from LHEF
  ExRootTreeBranch *branchParticle = treeWriter->NewBranch("Particle", TRootLHEFParticle::Class());

  ExRootTreeBranch *branchJet = treeWriter->NewBranch("Jet", TRootLHEFParticle::Class());
  std::vector<fastjet::PseudoJet> jets;

  cout << "** Calculating number of events to process. Please wait..." << endl;
  Long64_t allEntries = inputReader->getNumberOfEvents();
  cout << "** Input file contains " << allEntries << " events" << endl;

  const fastjet::JetDefinition& jet_def = getJetDefinition("");
  std::vector<fastjet::PseudoJet>::const_iterator pj;

  if(allEntries > 0)
  {
    ExRootProgressBar progressBar(allEntries);
    
    // Loop over all events
    Long64_t entry = 0;
    while(inputReader->readEvent())
    {
      treeWriter->Clear();

      AnalyseEvent(inputReader, branchEvent, entry + 1);
      AnalyseParticles(inputReader, branchParticle);

      /** Retrieve particle data from the branches and run jet algorithm */
      runJetAlgorithm(branchParticle, "LHEF", jet_def, jets);
      for (pj=jets.begin(); pj!=jets.end(); ++pj) {
	TRootLHEFParticle* element = 
	  dynamic_cast<TRootLHEFParticle*>(branchJet->NewEntry());
	element->PID = 0;
	element->Status = 1;
	element->Px = pj->px();
	element->Py = pj->py();
	element->Pz = pj->pz();
	element->E = pj->E();
	element->M = pj->m();
	element->PT = pj->perp();
	element->Phi = pj->phi();
	element->Eta = pj->eta();
	element->Rapidity = pj->rap();
      }
      treeWriter->Fill();

      progressBar.Update(entry);

      ++entry;
    }

    progressBar.Finish();
  }

  treeWriter->Write();

  cout << "** Exiting..." << endl;

  delete treeWriter;
  delete outputFile;
  delete inputReader;
}

