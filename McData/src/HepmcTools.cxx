/*
  HepmcTools.cxx
*/
#include <memory>
#include "McData/HepmcTools.hxx"
#include "McData/McVertex.hxx"
#include "McData/McParticle.hxx"
//#include "LHEF.h"
#include "ExRootAnalysis/ExRootClasses.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/FourVector.h"
#include "HepMC3/Attribute.h"


// #include "HepMC3/IO_GenEvent.h"
// #include "HepMC3/SimpleVector.h"

void convertEventData(const HepMC3::GenEvent& mcevent, int nparticles, 
		      ExRootTreeBranch* branch) {
  TRootLHEFEvent *element = dynamic_cast<TRootLHEFEvent*>(branch->NewEntry());

  //  const HepMC3::WeightContainer& weights = mcevent.weights();
  auto weights = mcevent.weights();
  double w = 1.0;
  std::shared_ptr<HepMC3::Attribute> attr;
  std::shared_ptr<HepMC3::IntAttribute> attri; 
  std::shared_ptr<HepMC3::DoubleAttribute> attrd;

  if (weights.size() > 0) w = weights[0];
  element->Number = mcevent.event_number();

  attri = mcevent.attribute<HepMC3::IntAttribute>("signal_process_id");
  if (attri.get() != nullptr) {
    element->ProcessID = attri->value();
  }
  element->Weight = w;
  attrd = mcevent.attribute<HepMC3::DoubleAttribute>("event_scale");
  if (attrd.get() != nullptr) {
    element->ScalePDF = attrd->value();
  }
  attrd = mcevent.attribute<HepMC3::DoubleAttribute>("alphaQED");
  if (attrd.get() != nullptr) {
    element->CouplingQED = attrd->value();
  }
  attrd = mcevent.attribute<HepMC3::DoubleAttribute>("alphaQCD");
  if (attrd.get() != nullptr) {
    element->CouplingQCD =attrd->value();
  }
  element->Nparticles = nparticles;
}

HepMC3::ConstGenParticlePtr 
findParticleDecayedAt(const HepMC3::ConstGenVertexPtr vertex, 
		      const HepMC3::GenEvent& mcevent, 
		      int& particle_index) {
  HepMC3::ConstGenParticlePtr parent=nullptr;
  //  HepMC3::GenEvent::particle_const_iterator p1=mcevent.particles_begin();

  particle_index=-1;
  int i=0;

  //  for (; p1!=mcevent.particles_end(); ++p1, ++i) {
  for (auto p1: mcevent.particles()) {
    HepMC3::ConstGenVertexPtr vtx = p1->end_vertex();
    if (vtx == vertex) {
      particle_index=i;
      parent = p1;
    }
  }
  return parent;
}

int convertParticleData(const HepMC3::GenEvent& mcevent, 
			ExRootTreeBranch* branch) {
  //  HepMC3::GenEvent::particle_const_iterator p1=mcevent.particles_begin();
  int ntotal=0;
  //  for (; p1!=mcevent.particles_end(); ++p1) {
  for (auto p1: mcevent.particles()) {
    ntotal++;
    TRootLHEFParticle* element = 
      dynamic_cast<TRootLHEFParticle*>(branch->NewEntry());
    element->PID = p1->pdg_id();
    element->Status = p1->status();
    HepMC3::FourVector v4 = p1->momentum();
    element->Px = v4.px();
    element->Py = v4.py();
    element->Pz = v4.pz();
    element->E = v4.e();
    element->M = v4.m();
    element->PT = v4.perp();
    element->Phi = v4.phi();
    element->Eta = v4.eta();
    element->Rapidity = log( (v4.e()+v4.pz())/(v4.e()-v4.pz()) );
    HepMC3::ConstGenVertexPtr prodvertex = p1->production_vertex();
    int iparent=-1;
    HepMC3::ConstGenParticlePtr parent = 
      findParticleDecayedAt(prodvertex, mcevent, iparent);
    element->Mother1 = iparent;
    element->Mother2 = -1;
    element->ColorLine1 = -1;
    element->ColorLine2 = -1;
  }
  return ntotal;
}

void convertEventData(const HepMC3::GenEvent& mcevent, 
		      TClonesArray* particles, 
		      TClonesArray* vertices) {
}

int convertParticleData(const HepMC3::GenEvent& mcevent, 
			TClonesArray* particles, 
			const std::map<HepMC3::ConstGenVertexPtr, int>& vtx_id_map) {
  int np=0;
  McParticle* particle=0;

  particles->Clear();
  for (auto p1: mcevent.particles()) {
    particle = new ( (*particles)[np]) McParticle();
    particle->setPDGID( p1->pdg_id());
    const HepMC3::FourVector& v4 = p1->momentum();
    particle->setMomentum(v4.px(), v4.py(), v4.pz(), v4.e());
    particle->setStatus(p1->status());
    
    auto vtx = p1->production_vertex();
    auto p_v = vtx_id_map.find(vtx);
    if (p_v != vtx_id_map.end()) {
      particle->setProdVertexIndex(p_v->second);
    } else {
      particle->setProdVertexIndex(-1);
    }
    vtx = p1->end_vertex();
    if ( (p_v = vtx_id_map.find(vtx)) != vtx_id_map.end()) {
      particle->setEndVertexIndex(p_v->second);
    } else {
      particle->setEndVertexIndex(-1);
    }
    //    const HepMC3::Polarization& pol = p1->polarization();
    // particle->setPolTheta(pol.theta());
    // particle->setPolPhi(pol.phi());
    std::shared_ptr<HepMC3::DoubleAttribute> attrd;
    // for (auto att: p1->attribute_names()) {
    //   std::cout << "att: " << att << std::endl;
    // }
    if ( (attrd = p1->attribute<HepMC3::DoubleAttribute>("spin") ) != nullptr) {
      if (attrd.get() != nullptr) {
	double spin = attrd->value();
	// if (spin < 8) {
	//   std::cout << "particle[" << ip << "] pdgid=" << particle->pdgID()
	// 	    << " spin: " << attrd->value() << std::endl;
	// }
	particle->setSpin(spin);
      }
    }
    if ( (attrd = p1->attribute<HepMC3::DoubleAttribute>("polTheta") ) != nullptr) {
      if (attrd.get() != nullptr) {
	particle->setPolTheta(attrd->value() );
      }
    }
    if ( (attrd = p1->attribute<HepMC3::DoubleAttribute>("polPhi") ) != nullptr) {
      if (attrd.get() != nullptr) {

	particle->setPolPhi(attrd->value() );
      }
    }
    np ++;
  }
  return 0;
}

int convertVertexData(const HepMC3::GenEvent& mcevent, 
		      TClonesArray* vertices, 
		      std::map<HepMC3::ConstGenVertexPtr, int>& vtx_id_map) {
  //  HepMC3::GenEvent::vertex_const_iterator p1=mcevent.vertices_begin();
  int nv=0;
  McVertex* vtx=0;

  vtx_id_map.clear();
  vertices->Clear();
  for (auto p1: mcevent.vertices() ) {
    const HepMC3::FourVector& v4 = p1->position();
    vtx = new ( (*vertices)[nv]) McVertex(v4.x(), v4.y(), v4.z(), v4.t());
    vtx->setPosition(v4.x(), v4.y(), v4.z(), v4.t());
    //    std::cout<< "vtx.z="<< v4.z()<< std::endl;
    vtx_id_map[p1] = nv;
    nv ++;
  }
  return 0;
}

