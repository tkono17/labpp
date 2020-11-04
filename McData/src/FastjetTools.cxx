/*
  FastjetTools.cxx
*/
#include "McData/FastjetTools.hxx"
#include "McData/Jet.hxx"
#include "McData/IndexPair.hxx"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "TClonesArray.h"

const fastjet::JetDefinition& getJetDefinition(const std::string& /*algo_type*/) {
  static fastjet::JetDefinition* jet_def(0);
  if (jet_def == 0) {
    // algo_type not used 
    double R=0.7;
    jet_def = new fastjet::JetDefinition(fastjet::antikt_algorithm, R);
  }
  return *jet_def;
}

fastjet::ClusterSequence* 
runJetAlgorithm(const HepMC3::GenEvent& event, 
		const fastjet::JetDefinition& jet_def, 
		std::vector<fastjet::PseudoJet>& jets) {
  std::vector<fastjet::PseudoJet> v;
  jets.clear();
  int ip=0;
  fastjet::PseudoJet pj;

  //  HepMC3::GenEvent::particle_const_iterator p1=event.particles_begin();
  //  for (; p1!=event.particles_end(); ++p1, ++ip) {
  for (auto p1: event.particles()) {
    int pid2 = abs( p1->pdg_id());
    int status =  p1->status();
    if (status == 1 && pid2 != 12 && pid2 != 14 && pid2 != 16) {
      // Stable particle and not neutrinos
      HepMC3::FourVector v4 =  p1->momentum();
      pj = fastjet::PseudoJet(v4.px(), v4.py(), v4.pz(), v4.e());
      pj.set_user_index(ip);
      v.push_back(pj);
    }
  }
  fastjet::ClusterSequence* cs = new fastjet::ClusterSequence(v, jet_def);
  jets = sorted_by_pt(cs->inclusive_jets());
  return cs;
}

void convertJetData(const std::vector<fastjet::PseudoJet>& jets, 
		    ExRootTreeBranch* branch) {
  std::vector<fastjet::PseudoJet>::const_iterator pj;
  for (pj=jets.begin(); pj!=jets.end(); ++pj) {
    Jet* element = dynamic_cast<Jet*>(branch->NewEntry());
    element->setMomentum(TLorentzVector(pj->px(), pj->py(), pj->pz(), pj->e()));
  }
}

void convertJetData(const std::vector<fastjet::PseudoJet>& jets, 
		    TClonesArray* ca_jets, 
		    TClonesArray* ca_relations) {
  std::vector<fastjet::PseudoJet>::const_iterator pj, pj2;
  Jet* jet=0;
  int njets=0;
  int nrel=0;
  IndexPair jp_match;

  ca_jets->Clear();
  ca_relations->Clear();

  for (pj=jets.begin(); pj!=jets.end(); ++pj, ++njets) {
    jet = new ( (*ca_jets)[njets]) Jet();
    jet->setMomentum(TLorentzVector(pj->px(), pj->py(), pj->pz(), pj->e()));
    std::vector<fastjet::PseudoJet> constituents = pj->constituents();
    for (pj2=constituents.begin(); pj2!=constituents.end(); ++pj2) {
      jp_match.set(njets, pj2->user_index());
      new ( (*ca_relations)[nrel++]) IndexPair(jp_match);
    }
  }
}

