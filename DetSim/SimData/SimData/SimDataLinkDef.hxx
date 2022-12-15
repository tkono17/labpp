#ifndef __SimData_SimDataLinkDef_hxx__
#define __SimData_SimDataLinkDef_hxx__
/*
  SimDataLinkDef.hxx
*/
#include <vector>
#include "SimData/SimVertex.hxx"
#include "SimData/SimParticle.hxx"
#include "SimData/SCHitData.hxx"
#include "SimData/SimEvent.hxx"

#ifdef __CINT__

#pragma link C++ namespace ds;
//#pragma link C++ nestedclass;
//#pragma link C++ nestedtypedef;
#pragma link C++ class ds::SimVertex+;
#pragma link C++ class ds::SimParticle+;
#pragma link C++ class ds::SCHitData+;
#pragma link C++ class ds::SimEvent+;

#pragma link C++ class std::vector<ds::SimVertex>+;
#pragma link C++ class std::vector<ds::SimParticle>+;

#endif // __CINT__

#endif // __SimData_SimDataLinkDef_hxx__
