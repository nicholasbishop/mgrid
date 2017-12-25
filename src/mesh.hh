#ifndef SRC_MESH_HH_
#define SRC_MESH_HH_

#include <map>
#include <vector>

#include <QUuid>
#include <QVector3D>

template<typename T>
class BaseId {
 public:
  QUuid id;
};

class Edge;
class Grid;

using EdgeId = BaseId<Edge>;
using GridId = BaseId<Grid>;

class Edge {
 public:
  std::pair<GridId, GridId> grids;
};

class Grid {
 public:
  int width;
  int height;
  std::vector<QVector3D> points;
};

class Mesh {
 public:
  std::map<EdgeId, Edge> edges;
  std::map<GridId, Grid> grids;
};

#endif  // SRC_MESH_HH_
