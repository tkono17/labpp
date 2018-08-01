/*
  McConverter.cxx
*/
#include "McData/McConverter.hxx"
#include "McData/McEvent.hxx"
#include "McData/McParticle.hxx"
#include "McData/McVertex.hxx"
#include "HepMC/GenEvent.h"
#include "HepMC/PdfInfo.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/SimpleVector.h"
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
    HepMC::GenVertex* genv = *iv;
    McVertex* v = dynamic_cast<McVertex*>(mVertices->ConstructedAt(i));
    convertVertex(*v, *genv);
    vertexmap[genv] = i;
  }

  for (i=0, ip=event.particles_begin(); ip!=event.particles_end(); ++ip) {
    HepMC::GenParticle* genp = *ip;
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
  out.setSignalProcessID(in.signal_process_id());
  out.setEventNumber(in.event_number());

  const HepMC::PdfInfo* pdf = in.pdf_info();
  out.setPDFInfo(0, pdf->id1(), pdf->pdf_id1(), pdf->x1(), pdf->pdf1());
  out.setPDFInfo(1, pdf->id2(), pdf->pdf_id2(), pdf->x2(), pdf->pdf2());
  out.setPDFscale(pdf->scalePDF());
  out.setSignalProcessID(in.signal_process_id());
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
