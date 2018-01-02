#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <glm/vec3.hpp>

#include "external/json.hpp"
#include "src/unique_key.hh"

class Vert;
class Edge;
class Quad;

using VertKey = UniqueKey<Vert>;
using EdgeKey = UniqueKey<Edge>;
using QuadKey = UniqueKey<Quad>;

using VertEdges = std::vector<EdgeKey>;
using EdgeVerts = std::array<VertKey, 2>;
using EdgeQuads = std::array<QuadKey, 2>;
using QuadVerts = std::array<VertKey, 4>;
using QuadEdges = std::array<EdgeKey, 4>;

class Vert {
 public:
  explicit Vert(const glm::vec3& loc);

  const VertEdges& edges() const { return edges_; }

  const glm::vec3& loc() const { return loc_; }

  void add_edge(EdgeKey ek);

  void remove_edge(EdgeKey ek);

 private:
  VertEdges edges_;
  glm::vec3 loc_;
};

class Edge {
 public:
  explicit Edge(const EdgeVerts& verts);

  const EdgeVerts& verts() const { return verts_; }
  const EdgeQuads& quads() const { return quads_; }

  bool has_vert(VertKey vk) const;

  void add_quad(QuadKey qk);

  void remove_quad(QuadKey qk);

 private:
  EdgeVerts verts_;
  EdgeQuads quads_;
};

class Quad {
 public:
  explicit Quad(const QuadVerts& verts);

  QuadVerts verts() const { return verts_; }

 private:
  QuadVerts verts_;
};

void to_json(nlohmann::json& j, const Vert& vert);
void to_json(nlohmann::json& j, const Quad& vert);

class Mesh {
 public:
  using VertMap = std::unordered_map<VertKey, Vert>;
  using EdgeMap = std::unordered_map<EdgeKey, Edge>;
  using QuadMap = std::unordered_map<QuadKey, Quad>;

  static Mesh cube();

  const VertMap verts() const { return verts_; }
  const EdgeMap edges() const { return edges_; }
  const QuadMap quads() const { return quads_; }

  EdgeKey get_edge(const EdgeVerts& verts);
  EdgeKey get_or_add_edge(const EdgeVerts& verts);

  std::array<Vert*, 2> get_edge_verts(EdgeKey ek);
  std::array<Vert*, 4> get_quad_verts(QuadKey qk);
  std::array<Edge*, 4> get_quad_edges(QuadKey qk);
  QuadEdges get_quad_edge_keys(QuadKey qk);

  VertKey add_vert(const glm::vec3& loc);
  EdgeKey add_edge(const EdgeVerts& verts);
  QuadKey add_quad_from_verts(const QuadVerts& verts);

  void remove_edge(EdgeKey ek);
  void remove_quad(QuadKey qk);

  void subdivide();

  void validate();

  void check_for_loose_edges();

 private:
  VertMap verts_;
  EdgeMap edges_;
  QuadMap quads_;

  VertKey current_vert_key_;
  EdgeKey current_edge_key_;
  QuadKey current_quad_key_;
};
