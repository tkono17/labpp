#ifndef __MyMergingHooks_hxx__
#define __MyMergingHooks_hxx__
/*
  MyMergingHooks.hxx
*/
#include "Pythia8/Pythia.h"

class MyMergingHooks : public Pythia8::MergingHooks {

private:

public:

  // Default constructor
  MyMergingHooks();
  // Destructor
  ~MyMergingHooks();

  // Functional definition of the merging scale
  virtual double tmsDefinition( const Pythia8::Event& event);

  // Helper function for tms definition
  double myKTdurham(const Pythia8::Particle& RadAfterBranch,
		    const Pythia8::Particle& EmtAfterBranch, 
		    int Type, double D );

};

#endif // __MyMergingHooks_hxx__
