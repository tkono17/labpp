#ifndef __FastjetTools_hxx__
#define __FastjetTools_hxx__
/*
  FastjetTools.hxx
*/
#include <string>
#include <vector>
#include "fastjet/ClusterSequence.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"
#include "HepMC/GenEvent.h"
#include "ExRootAnalysis/ExRootTreeBranch.h"

const fastjet::JetDefinition& getJetDefinition(const std::string& algo_type);

fastjet::ClusterSequence* 
runJetAlgorithm(const HepMC::GenEvent& event, 
		const fastjet::JetDefinition& jet_def, 
		std::vector<fastjet::PseudoJet>& jets);

void convertJetData(const std::vector<fastjet::PseudoJet>& jets, 
		    ExRootTreeBranch* branch);

void convertJetData(const std::vector<fastjet::PseudoJet>& jets, 
		    TClonesArray* ca_jets, 
		    TClonesArray* ca_relations);

#endif // __FastjetTools_hxx__
