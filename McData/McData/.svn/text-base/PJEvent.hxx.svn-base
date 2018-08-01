#ifndef __PJEvent_hxx__
#define __PJEvent_hxx__
/*
  PJEvent.hxx
*/
#include "TTree.h"
#include "TClonesArray.h"
#include "McData/McVertex.hxx"
#include "McData/McParticle.hxx"
#include "McData/Jet.hxx"
#include "McData/IndexPair.hxx"


class PJEvent : public TObject {
public:
  PJEvent();
  ~PJEvent();

  int nVertices() const;
  int nParticles() const;
  int nJets() const;
  int nJetParticleMatch() const;

  const McVertex* vertex(int i) const;
  const McParticle* particle(int i) const;
  const Jet* jet(int i) const;
  const IndexPair* jetParticleMatch(int i) const;
  const McVertex* findProdVertex(const McParticle* i) const;

  std::vector<const McParticle*> findMother(const McParticle* i) const; 
  std::vector<const McParticle*> findChildren(const McParticle* i) const; 
  std::vector<const McParticle*> descendants(const McParticle* i)const; 
  std::vector<int> particlesInJet(int jindex) const;

  void setBranchAddress(TTree* t);

protected:
  TClonesArray* mVertices;
  TClonesArray* mParticles;
  TClonesArray* mJets;
  TClonesArray* mJetParticleMatch;
};

#endif // __PJEvent_hxx__

