#ifndef __TrackVertex_Vertex_hxx__
#define __TrackVertex_Vertex_hxx__
/*
  Vertex.hxx
*/
#include <cstdint>
#include <list>
#include "TrackVertex/Point.hxx"
#include "TrackVertex/Track.hxx"

class Vertex {
public:
  Vertex();
  ~Vertex();
  
protected:
  Point mPosition;
  std::list<Track*> mTracks;
};

#endif  // __TrackVertex_Vertex_hxx__
