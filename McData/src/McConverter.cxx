/*
  McConverter.cxx
*/
#include "McData/McConverter.hxx"
#include "McData/McEvent.hxx"
#include "McData/McParticle.hxx"
#include "McData/McVertex.hxx"
#include "HepMC/GenEvent.h"
#include "HepMC/GenPdfInfo.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/FourVector.h"
#include "HepMC/Attribute.h"
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

void McConverter::convert(const HepMC::GenEvent& event) {
  clearEvent();

  convertEvent(*mEvent, event);
  
  HepMC::GenEvent::particle_const_iterator ip;
  HepMC::GenEvent::vertex_const_iterator iv;
  std::map<const void*,int> vertexmap; // hold a pointer->index mapping
  std::map<const void*,int>::const_iterator vtxentry;

  int i=0;
  for (iv=event.vertices_begin(); iv!=event.vertices_end(); ++iv, ++i) {
    HepMC::GenVertexPtr genv = *iv;
    McVertex* v = dynamic_cast<McVertex*>(mVertices->ConstructedAt(i));
    convertVertex(*v, *genv);
    vertexmap[genv] = i;
  }

  for (i=0, ip=event.particles_begin(); ip!=event.particles_end(); ++ip) {
    HepMC::GenParticlePtr genp = *ip;
    McParticle* p = dynamic_cast<McParticle*>(mParticles->ConstructedAt(i));
    convertParticle(*p, *genp);
    const HepMC::GenVertex* vtx1 = genp->production_vertex();
    const HepMC::GenVertex* vtx2 = genp->end_vertex();
    if (vtx1 && (vtxentry=vertexmap.find(vtx1)) != vertexmap.end()) {
      p->setProdVertexIndex(vtxentry->second);
    }
    if (vtx2 && (vtxentry=vertexmap.find(vtx2)) != vertexmap.end()) {
      p->setEndVertexIndex(vtxentry->second);
    }
  }

}

void McConverter::convertEvent(McEvent& out, const HepMC::GenEvent& in) {
  const HepMC::GenPdfInfoPtr pdf = in.pdf_info();
  out.setPDFInfo(0, pdf->parton_id[0], pdf->pdf_id[0], pdf->x[0], pdf->xf[0]);
  out.setPDFInfo(1, pdf->parton_id[1], pdf->pdf_id[1], pdf->x[1], pdf->xf[1]);
  out.setPDFscale(pdf->scale);

  std::shared_ptr<HepMC::IntAttribute> attri;
  attri = in.attribute<HepMC::IntAttribute>("signal_process_id");
  if (attri.get() != nullptr) {
    out.setSignalProcessID(attri->value());
  }
  out.setEventNumber(in.event_number());
}

void McConverter::convertParticle(McParticle& out, 
				  const HepMC::GenParticle& in) {
  const HepMC::FourVector& v4 = in.momentum();
  out.setPDGID(in.pdg_id());
  out.setMomentum(v4.px(), v4.py(), v4.pz(), v4.e());
}

void McConverter::convertVertex(McVertex& out, const HepMC::GenVertex& in) {
  const HepMC::FourVector& pos = in.position();
  out.setPosition(pos.x(), pos.y(), pos.z(), pos.t());
}


void McConverter::clearEvent() {
  mParticles->Clear();
  mVertices->Clear();
}
