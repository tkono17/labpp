/*
  McConverter.cxx
*/
#include "McData/McConverter.hxx"
#include "McData/McEvent.hxx"
#include "McData/McParticle.hxx"
#include "McData/McVertex.hxx"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenPdfInfo.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/FourVector.h"
#include "HepMC3/Attribute.h"
#include "TTree.h"
#include "TClonesArray.h"

using namespace std;

McConverter::McConverter() : mTree(0), mEvent(0), mParticles(0), mVertices(0) {
}

McConverter::~McConverter() {
}

TTree* McConverter::initTree(const std::string& name, const std::string& title) {
  if (mTree) delete mTree;
  mTree = new TTree(name.c_str(), title.c_str());

  mEvent = new McEvent();
  mParticles = new TClonesArray("McParticle", 1000);
  mVertices = new TClonesArray("McVertex", 1000);

  mTree->Branch("event", "McEvent", &mEvent);
  mTree->Branch("particles", "TClonesArray", &mParticles);
  mTree->Branch("vertices", "TClonesArray", &mVertices);

  return mTree;
}

void McConverter::convert(const HepMC3::GenEvent& event) {
  clearEvent();

  convertEvent(*mEvent, event);
  
  //  HepMC3::GenEvent::particle_const_iterator ip;
  //  HepMC3::GenEvent::vertex_const_iterator iv;
  std::map<const void*,int> vertexmap; // hold a pointer->index mapping
  std::map<const void*,int>::const_iterator vtxentry;

  int i=0;
  for (auto iv: event.vertices() ) {
    //  for (iv=event.vertices_begin(); iv!=event.vertices_end(); ++iv, ++i) {
    HepMC3::ConstGenVertexPtr genv = iv;
    McVertex* v = dynamic_cast<McVertex*>(mVertices->ConstructedAt(i));
    convertVertex(*v, *genv);
    vertexmap[genv.get()] = i;
    i++;
  }

  //  for (i=0, ip=event.particles_begin(); ip!=event.particles_end(); ++ip) {
  for (auto ip: event.particles()) {
    HepMC3::ConstGenParticlePtr genp = ip;
    McParticle* p = dynamic_cast<McParticle*>(mParticles->ConstructedAt(i));
    convertParticle(*p, *genp);
    HepMC3::ConstGenVertexPtr vtx1 = genp->production_vertex();
    HepMC3::ConstGenVertexPtr vtx2 = genp->end_vertex();
    if (vtx1 && (vtxentry=vertexmap.find(vtx1.get())) != vertexmap.end()) {
      p->setProdVertexIndex(vtxentry->second);
    }
    if (vtx2 && (vtxentry=vertexmap.find(vtx2.get())) != vertexmap.end()) {
      p->setEndVertexIndex(vtxentry->second);
    }
    i ++;
  }

}

void McConverter::convertEvent(McEvent& out, const HepMC3::GenEvent& in) {
  HepMC3::ConstGenPdfInfoPtr pdf = in.pdf_info();
  out.setPDFInfo(0, pdf->parton_id[0], pdf->pdf_id[0], pdf->x[0], pdf->xf[0]);
  out.setPDFInfo(1, pdf->parton_id[1], pdf->pdf_id[1], pdf->x[1], pdf->xf[1]);
  out.setPDFscale(pdf->scale);

  std::shared_ptr<HepMC3::IntAttribute> attri;
  attri = in.attribute<HepMC3::IntAttribute>("signal_process_id");
  if (attri.get() != nullptr) {
    out.setSignalProcessID(attri->value());
  }
  out.setEventNumber(in.event_number());
}

void McConverter::convertParticle(McParticle& out, 
				  const HepMC3::GenParticle& in) {
  const HepMC3::FourVector& v4 = in.momentum();
  out.setPDGID(in.pdg_id());
  out.setMomentum(v4.px(), v4.py(), v4.pz(), v4.e());
}

void McConverter::convertVertex(McVertex& out, const HepMC3::GenVertex& in) {
  const HepMC3::FourVector& pos = in.position();
  out.setPosition(pos.x(), pos.y(), pos.z(), pos.t());
}


void McConverter::clearEvent() {
  mParticles->Clear();
  mVertices->Clear();
}
