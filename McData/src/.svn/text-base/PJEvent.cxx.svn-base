/*
  PJEvent.cxx
*/
#include "McData/PJEvent.hxx"
#include <algorithm>
#include <iterator>

PJEvent::PJEvent() : TObject() {
  mVertices = 0;
  mParticles = 0;
  mJets = 0;
  mJetParticleMatch = 0;
}

PJEvent::~PJEvent() {
  if (mVertices) {
    delete mVertices;
    mVertices = 0;
  }
  if (mParticles) {
    delete mParticles;
    mParticles = 0;
  }
  if (mJets) {
    delete mJets;
    mJets = 0;
  }
  if (mJetParticleMatch) {
    delete mJetParticleMatch;
    mJetParticleMatch = 0;
  }
}

int PJEvent::nVertices() const {
  return mVertices->GetEntries();
}

int PJEvent::nParticles() const {
  return mParticles->GetEntries();
}

int PJEvent::nJets() const {
  return mJets->GetEntries();
}

int PJEvent::nJetParticleMatch() const {
  return mJetParticleMatch->GetEntries();
}


const McVertex* PJEvent::vertex(int i) const {
  const McVertex* x = dynamic_cast<McVertex*>(mVertices->At(i));
  return x;
}

const McParticle* PJEvent::particle(int i) const {
  const McParticle* x = dynamic_cast<McParticle*>(mParticles->At(i));
  return x;
}

const Jet* PJEvent::jet(int i) const {
  const Jet* x = dynamic_cast<Jet*>(mJets->At(i));
  return x;
}

const IndexPair* PJEvent::jetParticleMatch(int i) const {
  const IndexPair* x = dynamic_cast<IndexPair*>(mJetParticleMatch->At(i));
  return x;
}

const McVertex* PJEvent::findProdVertex(const McParticle* i) const {
  int p = i->prodVertexIndex();
  return vertex(p);
}

std::vector<const McParticle*> PJEvent::findMother(const McParticle* i) const {
 std::vector<const McParticle*> v;
 int pvi = i->prodVertexIndex();
 
 for(int j =0; j<nParticles(); ++j){
 const McParticle* x = particle(j); 
 int evi = x->endVertexIndex();

 if(pvi == evi){
   v.push_back(x);
 }

 }

 return v;
}


std::vector<const McParticle*> PJEvent::findChildren(const McParticle* i) const {
 std::vector<const McParticle*> v;
 int evi = i->endVertexIndex();
 
 for(int j =0; j<nParticles(); ++j){
 const McParticle* x = particle(j); 
 int pvi = x->prodVertexIndex();

 if(evi == pvi){
   v.push_back(x);
 }

 }

 return v;
}

std::vector<const McParticle*> PJEvent::descendants(const McParticle* i) const {
  std::vector<const McParticle*> v;
  
  std::vector<const McParticle*> v2 =findChildren(i);
  for(int j=0;j<v2.size();++j){
    const McParticle* x = v2[j];
    int end = x->endVertexIndex();
    if(end == -1){
      v.push_back(x);
    }
    else{
      std::vector<const McParticle*> v3 = descendants(x);
      std::copy(v3.begin(),v3.end(),std::back_inserter(v));
    }
 }
  return v; 
}


std::vector<int> PJEvent::particlesInJet(int jindex) const {
  std::vector<int> v;
  int n = nJetParticleMatch();
  for (int i=0; i<n; ++i) {
    const IndexPair* x = jetParticleMatch(i);
    if (x->first() == jindex) {
      v.push_back(x->second());
    }
  }
  return v;
}

void PJEvent::setBranchAddress(TTree* t) {
  mVertices = new TClonesArray("McVertex", 1000);
  mParticles = new TClonesArray("McParticle", 1000);
  mJets = new TClonesArray("Jet", 1000);
  mJetParticleMatch = new TClonesArray("IndexPair", 1000);
  t->SetBranchAddress("Vertex", &mVertices);
  t->SetBranchAddress("Particle", &mParticles);
  t->SetBranchAddress("Jet", &mJets);
  t->SetBranchAddress("Jet_match", &mJetParticleMatch);
}


//  LocalWords:  PJEvent
