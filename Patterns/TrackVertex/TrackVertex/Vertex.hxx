#ifndef __TrackVertex_Vertex_hxx__
#define __TrackVertex_Vertex_hxx__
/*
  Vertex.hxx
*/
#include <cstdint>
#include <list>
#include "TObject.h"
#include "TrackVertex/Point.hxx"

class Vertex : public TObject {
public:
  Vertex();
  Vertex(double vx, double vy);
  ~Vertex();

  const Point& position() const { return mPosition; }
  
  ClassDef(Vertex, 1)
  
protected:
  Point mPosition;
  //  std::list<Track*> mTracks;
};

#endif  // __TrackVertex_Vertex_hxx__
