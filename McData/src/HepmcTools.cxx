/*
  HepmcTools.cxx
*/
#include "McData/HepmcTools.hxx"
#include "McData/McVertex.hxx"
#include "McData/McParticle.hxx"
#include "LHEF.h"
#include "ExRootAnalysis/ExRootClasses.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/Polarization.h"
#include "HepMC/SimpleVector.h"


// #include "HepMC/IO_GenEvent.h"
// #include "HepMC/SimpleVector.h"

void convertEventData(const HepMC::GenEvent& mcevent, int nparticles, 
		      ExRootTreeBranch* branch) {
  TRootLHEFEvent *element = dynamic_cast<TRootLHEFEvent*>(branch->NewEntry());

  const HepMC::WeightContainer& weights = mcevent.weights();
  double w = 1.0;
  if (weights.size() > 0) w = weights[0];
  element->Number = mcevent.event_number();
  //  element->Nparticles = 
  element->ProcessID = mcevent.signal_process_id();
  element->Weight = w;
  element->ScalePDF = mcevent.event_scale();
  element->CouplingQED = mcevent.alphaQED();
  element->CouplingQCD = mcevent.alphaQCD();
  element->Nparticles = nparticles;
}
const HepMC::GenParticle* 
findParticleDecayedAt(const HepMC::GenVertex* vertex, 
		      const HepMC::GenEvent& mcevent, 
		      int& particle_index) {
  const HepMC::GenParticle* parent=0;
  HepMC::GenEvent::particle_const_iterator p1=mcevent.particles_begin();
  particle_index=-1;
  int i=0;
  for (; p1!=mcevent.particles_end(); ++p1, ++i) {
    const HepMC::GenVertex* vtx = (*p1)->end_vertex();
    if (vtx == vertex) {
      particle_index=i;
      parent = *p1;
    }
  }
  return parent;
}

int convertParticleData(const HepMC::GenEvent& mcevent, 
			ExRootTreeBranch* branch) {
  HepMC::GenEvent::particle_const_iterator p1=mcevent.particles_begin();
  int ntotal=0;
  for (; p1!=mcevent.particles_end(); ++p1) {
    ntotal++;
    TRootLHEFParticle* element = 
      dynamic_cast<TRootLHEFParticle*>(branch->NewEntry());
    element->PID = (*p1)->pdg_id();
    element->Status = (*p1)->status();
    HepMC::FourVector v4 = (*p1)->momentum();
    element->Px = v4.px();
    element->Py = v4.py();
    element->Pz = v4.pz();
    element->E = v4.e();
    element->M = v4.m();
    element->PT = v4.perp();
    element->Phi = v4.phi();
    element->Eta = v4.eta();
    element->Rapidity = log( (v4.e()+v4.pz())/(v4.e()-v4.pz()) );
    const HepMC::GenVertex* prodvertex = (*p1)->production_vertex();
    int iparent=-1;
    const HepMC::GenParticle* parent = 
      findParticleDecayedAt(prodvertex, mcevent, iparent);
    element->Mother1 = iparent;
    element->Mother2 = -1;
    element->ColorLine1 = -1;
    element->ColorLine2 = -1;
  }
  return ntotal;
}

void convertEventData(const HepMC::GenEvent& mcevent, 
		      TClonesArray* particles, 
		      TClonesArray* vertices) {
}

int convertParticleData(const HepMC::GenEvent& mcevent, 
			TClonesArray* particles, 
			const std::map<HepMC::GenVertex*, int>& vtx_id_map) {
  HepMC::GenEvent::particle_const_iterator p1=mcevent.particles_begin();
  int np=0;
  McParticle* particle=0;
  HepMC::GenVertex* vtx=0;
  std::map<HepMC::GenVertex*, int>::const_iterator p_v;

  particles->Clear();
  for (; p1!=mcevent.particles_end(); ++p1, ++np) {
    particle = new ( (*particles)[np]) McParticle();
    particle->setPDGID( (*p1)->pdg_id());
    const HepMC::FourVector& v4 = (*p1)->momentum();
    particle->setMomentum(v4.px(), v4.py(), v4.pz(), v4.e());
    particle->setStatus((*p1)->status());
    
    vtx = (*p1)->production_vertex();
    if ( (p_v = vtx_id_map.find(vtx)) != vtx_id_map.end()) {
      particle->setProdVertexIndex(p_v->second);
    } else {
      particle->setProdVertexIndex(-1);
    }
    vtx = (*p1)->end_vertex();
    if ( (p_v = vtx_id_map.find(vtx)) != vtx_id_map.end()) {
      particle->setEndVertexIndex(p_v->second);
    } else {
      particle->setEndVertexIndex(-1);
    }
    const HepMC::Polarization& pol = (*p1)->polarization();
    particle->setPolTheta(pol.theta());
    particle->setPolPhi(pol.phi());
  }
  return 0;
}

int convertVertexData(const HepMC::GenEvent& mcevent, 
		      TClonesArray* vertices, 
		      std::map<HepMC::GenVertex*, int>& vtx_id_map) {
  HepMC::GenEvent::vertex_const_iterator p1=mcevent.vertices_begin();
  int nv=0;
  McVertex* vtx=0;

  vtx_id_map.clear();
  vertices->Clear();
  for (; p1!=mcevent.vertices_end(); ++p1, ++nv) {
    const HepMC::FourVector& v4 = (*p1)->position();
    vtx = new ( (*vertices)[nv]) McVertex(v4.x(), v4.y(), v4.z(), v4.t());
    vtx->setPosition(v4.x(), v4.y(), v4.z(), v4.t());
    //    std::cout<< "vtx.z="<< v4.z()<< std::endl;
    vtx_id_map[*p1] = nv;
  }
  return 0;
}

