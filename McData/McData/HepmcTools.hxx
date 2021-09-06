#ifndef __HepmcTools_hxx__
#define __HepmcTools_hxx__
/*
  HepmcTools.hxx
*/
#include <map>
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "ExRootAnalysis/ExRootTreeBranch.h"
#include "TClonesArray.h"
#include "McData/McVertex.hxx"
#include "McData/McParticle.hxx"

// Conversion to data structure in ExRootAnalysis
//--------------------------------------------------------------------
void convertEventData(const HepMC3::GenEvent& mcevent, int nparticles, 
		      ExRootTreeBranch* branch);

int convertParticleData(const HepMC3::GenEvent& mcevent, 
			ExRootTreeBranch* branch);

HepMC3::ConstGenParticlePtr 
findParticleDecayedAt(const HepMC3::ConstGenVertexPtr vertex, 
		      const HepMC3::GenEvent& mcevent, 
		      int& particle_index);

// Conversion to data structure in McData
//--------------------------------------------------------------------
void convertEventData(const HepMC3::GenEvent& mcevent, 
		      TClonesArray* particles, 
		      TClonesArray* vertices);

int convertParticleData(const HepMC3::GenEvent& mcevent, 
			TClonesArray* particles, 
			const std::map<HepMC3::ConstGenVertexPtr, int>& vtx_id_map);

int convertVertexData(const HepMC3::GenEvent& mcevent, 
		      TClonesArray* vertices, 
		      std::map<HepMC3::ConstGenVertexPtr, int>& vtx_id_map);

#endif // __HepmcTools_hxx__
