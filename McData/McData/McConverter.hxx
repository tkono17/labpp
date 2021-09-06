/*
  McConverter.hxx
*/
#include "McData/McEvent.hxx"

namespace HepMC3 {
  class GenEvent;
  class GenParticle;
  class GenVertex;
}
class McEvent;
class McParticle;
class McVertex;

class TTree;
class TClonesArray;

class McConverter {
public:
  McConverter();
  ~McConverter();

  TTree* tree() { return mTree; }

  TTree* initTree(const std::string& name, const std::string& title);
  void convert(const HepMC3::GenEvent& event);
  void clearEvent();

private:
  void convertEvent(McEvent& out, const HepMC3::GenEvent& in);
  void convertParticle(McParticle& out, const HepMC3::GenParticle& in);
  void convertVertex(McVertex& out, const HepMC3::GenVertex& in);
private:
  TTree* mTree;

  /** Data structure to be dumped into TTree */
  McEvent* mEvent;
  TClonesArray* mParticles;
  TClonesArray* mVertices;

};
