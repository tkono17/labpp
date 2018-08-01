/*
  MyMergingHooks.cxx
*/
#include "ExPythia/MyMergingHooks.hxx"

using namespace Pythia8;

//--------------------------------------------------------------------------

// Constructor
MyMergingHooks::MyMergingHooks() {}

// Destructor
MyMergingHooks::~MyMergingHooks() {}

//--------------------------------------------------------------------------

// Definition of the merging scale

double MyMergingHooks::tmsDefinition( const Event& event){

  // Cut only on QCD partons!
  // Count particle types
  int nFinalColoured = 0;
  int nFinalNow =0;
  for( int i=0; i < event.size(); ++i) {
    if(event[i].isFinal()){
      if(event[i].id() != 23 && abs(event[i].id()) != 24)
        nFinalNow++;
      if( event[i].colType() != 0)
        nFinalColoured++;
    }
  }

  // Use MergingHooks in-built functions to get information on the hard process
  int nLeptons = nHardOutLeptons();
  int nQuarks  = nHardOutPartons();
  int nResNow  = nResInCurrent();

  // Check if photons, electrons etc. have been produced. If so, do not veto
  if(nFinalNow - ( (nLeptons+nQuarks)/2 - nResNow)*2 != nFinalColoured){
    // Sometimes, Pythia detaches the decay products even though no
    // resonance was put into the LHE file, to catch this, add another
    // if statement
    if(nFinalNow != nFinalColoured) return 0.;
  }

  // Check that one parton has been produced. If not (e.g. in MPI), do not veto
  int nMPI = infoPtr->nMPI();
  if(nMPI > 1) return 0.;

  // Declare kT algorithm parameters
  double Dparam = 0.4;
  int kTtype = -1;
  // Declare final parton vector
  vector <int> FinalPartPos;
  FinalPartPos.clear();
  // Search event record for final state partons
  for (int i=0; i < event.size(); ++i)
    if(event[i].isFinal() && event[i].colType() != 0)
      FinalPartPos.push_back(i);

  // Find minimal Durham kT in event, using own function: Check
  // definition of separation
  int type = (event[3].colType() == 0 && event[4].colType() == 0) ? 1 : kTtype;
  // Find minimal kT
  double ktmin = event[0].e();
  for(int i=0; i < int(FinalPartPos.size()); ++i){
    double kt12  = ktmin;
    // Compute separation to the beam axis for hadronic collisions
    if(type == -1 || type == -2) {
      double temp = event[FinalPartPos[i]].pT();
      kt12 = min(kt12, temp);
    }
    // Compute separation to other final state jets
    for(int j=i+1; j < int(FinalPartPos.size()); ++j) {
      double temp = kTdurham( event[FinalPartPos[i]], event[FinalPartPos[j]],
                      type, Dparam);
      kt12 = min(kt12, temp);
    }
    // Keep the minimal Durham separation
    ktmin = min(ktmin,kt12);
  }

  // Return minimal Durham kT
  return ktmin;

}

//--------------------------------------------------------------------------

// Function to compute durham y separation from Particle input

double MyMergingHooks::myKTdurham(const Particle& RadAfterBranch,
  const Particle& EmtAfterBranch, int Type, double D ){

  // Declare return variable
  double ktdur;
  // Save 4-momenta of final state particles
  Vec4 jet1 = RadAfterBranch.p();
  Vec4 jet2 = EmtAfterBranch.p();

  if( Type == 1) {
    // Get angle between jets for e+e- collisions, make sure that
    // -1 <= cos(theta) <= 1
    double costh;
    if (jet1.pAbs()*jet2.pAbs() <=0.) costh = 1.;
    else {
      costh = costheta(jet1,jet2);
    }
    // Calculate kt durham separation between jets for e+e- collisions
    ktdur = 2.0*min( pow(jet1.e(),2) , (pow(jet2.e(),2)) )*(1.0 - costh);
  } else if( Type == -1 ){
    // Get delta_eta and cosh(Delta_eta) for hadronic collisions
    double eta1 = 0.5*log( (jet1.e() + jet1.pz()) / (jet1.e() - jet1.pz()) );
    double eta2 = 0.5*log( (jet2.e() + jet2.pz()) / (jet2.e() - jet2.pz()) );
    // Get delta_phi and cos(Delta_phi) for hadronic collisions
    double pt1 = sqrt( pow(jet1.px(),2) + pow(jet1.py(),2) );
    double pt2 = sqrt( pow(jet2.px(),2) + pow(jet2.py(),2) );
    double cosdPhi = ( jet1.px()*jet2.px() + jet1.py()*jet2.py() ) / (pt1*pt2);
    double dPhi = acos( cosdPhi );
    // Calculate kT durham like fastjet
     ktdur = min( pow(pt1,2),pow(pt2,2) )
           * ( pow(eta1-eta2,2) + pow(dPhi,2) ) / pow(D,2);
  } else if( Type == -2 ){
    // Get delta_eta and cosh(Delta_eta) for hadronic collisions
    double eta1 = 0.5*log( (jet1.e() + jet1.pz()) / (jet1.e() - jet1.pz()) );
    double eta2 = 0.5*log( (jet2.e() + jet2.pz()) / (jet2.e() - jet2.pz()) );
     double coshdEta = cosh( eta1 - eta2 );
    // Get delta_phi and cos(Delta_phi) for hadronic collisions
    double pt1 = sqrt( pow(jet1.px(),2) + pow(jet1.py(),2) );
    double pt2 = sqrt( pow(jet2.px(),2) + pow(jet2.py(),2) );
    double cosdPhi = ( jet1.px()*jet2.px() + jet1.py()*jet2.py() ) / (pt1*pt2);
    // Calculate kT durham separation "SHERPA-like"
     ktdur = 2.0*min( pow(pt1,2),pow(pt2,2) )
           * ( coshdEta - cosdPhi ) / pow(D,2);
  } else {
    ktdur = 0.0;
  }
  // Return kT
  return sqrt(ktdur);
}

//==========================================================================
