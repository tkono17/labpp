#ifndef __HepmcTools_hxx__
#define __HepmcTools_hxx__
/*
  HepmcTools.hxx
*/
#include <map>
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "ExRootAnalysis/ExRootTreeBranch.h"
#include "TClonesArray.h"
#include "McData/McVertex.hxx"
#include "McData/McParticle.hxx"

// Conversion to data structure in ExRootAnalysis
//--------------------------------------------------------------------
void convertEventData(const HepMC::GenEvent& mcevent, int nparticles, 
		      ExRootTreeBranch* branch);

int convertParticleData(const HepMC::GenEvent& mcevent, 
			ExRootTreeBranch* branch);

const HepMC::GenParticle* 
findParticleDecayedAt(const HepMC::GenVertex* vertex, 
		      const HepMC::GenEvent& mcevent, 
		      int& particle_index);

// Conversion to data structure in McData
//--------------------------------------------------------------------
void convertEventData(const HepMC::GenEvent& mcevent, 
		      TClonesArray* particles, 
		      TClonesArray* vertices);

int convertParticleData(const HepMC::GenEvent& mcevent, 
			TClonesArray* particles, 
			const std::map<HepMC::GenVertex*, int>& vtx_id_map);

int convertVertexData(const HepMC::GenEvent& mcevent, 
		      TClonesArray* vertices, 
		      std::map<HepMC::GenVertex*, int>& vtx_id_map);

#endif // __HepmcTools_hxx__
