#include "mesh.hh"

#include <iostream>

using glm::vec3;
using std::array;
using std::unordered_map;
using std::unordered_set;

namespace {

template<typename C>
vec3 average_vert_locs(C& container) {
  vec3 v{0};
  for (const auto& vert : container) {
    v += vert->loc();
  }
  return v / static_cast<float>(container.size());
}

}  // namespace

Vert::Vert(const vec3& loc) : loc_(loc) {}

void Vert::add_edge(const EdgeKey ek) {
  edges_.emplace_back(ek);
}

void Vert::remove_edge(EdgeKey ek) {
  for (auto iter = edges_.begin(); iter != edges_.end(); ++iter) {
    if (*iter == ek) {
      edges_.erase(iter);
      return;
    }
  }
}

bool Edge::has_vert(const VertKey vk) const {
  return vk == verts_[0] || vk == verts_[1];
}

void Edge::add_quad(const QuadKey qk) {
  if (!quads_[0].valid()) {
    quads_[0] = qk;
  } else if (!quads_[1].valid()) {
    quads_[1] = qk;
  } else {
    throw std::runtime_error("too many quads per edge");
  }
}

void Edge::remove_quad(QuadKey qk) {
  if (quads_[0] == qk) {
    quads_[0] = QuadKey{};
  } else if (quads_[1] == qk) {
    quads_[1] = QuadKey{};
  } else {
    throw std::runtime_error("quad is not adjacent to edge");
  }
}

Edge::Edge(const EdgeVerts& verts) : verts_(verts) {}

Quad::Quad(const QuadVerts& verts) : verts_(verts) {}

void to_json(nlohmann::json& j, const Vert& vert) {
  j = {vert.loc().x, vert.loc().y, vert.loc().z};
}

void to_json(nlohmann::json& j, const Quad& quad) {
  j = quad.verts();
}

Mesh Mesh::cube() {
  Mesh mesh;

  // 3__________7
  // │╲         ╲
  // │ ╲       │ ╲
  // │  ╲_________╲
  // │  │2     |  │6
  // │  │ _____│  │
  // 0╲ │      4╲ │
  //   ╲│_________│
  //    1          5

  // clang-format off
  const array<VertKey, 8> verts = {
      // Left
      mesh.add_vert({-1, -1, -1}),
      mesh.add_vert({-1, -1,  1}),
      mesh.add_vert({-1,  1,  1}),
      mesh.add_vert({-1,  1, -1}),
      // Right
      mesh.add_vert({ 1, -1, -1}),
      mesh.add_vert({ 1, -1,  1}),
      mesh.add_vert({ 1,  1,  1}),
      mesh.add_vert({ 1,  1, -1}),
  };
  // clang-format on

  const array<array<int, 4>, 6> quad_vert_patterns{
      0, 1, 2, 3,  // left
      4, 7, 6, 5,  // right
      2, 6, 7, 3,  // top
      1, 0, 4, 5,  // bottom
      1, 5, 6, 2,  // front
      0, 3, 7, 4,  // back
  };

  for (const auto& pattern : quad_vert_patterns) {
    mesh.add_quad_from_verts({
        // clang-format off
        verts[pattern[0]],
        verts[pattern[1]],
        verts[pattern[2]],
        verts[pattern[3]],
        // clang-format on
    });
  }

  return mesh;
}

EdgeKey Mesh::get_edge(const EdgeVerts& verts) {
  const auto vk0 = verts.at(0);
  const auto vk1 = verts.at(1);
  const auto& v0 = verts_.at(vk0);
  for (const auto ek : v0.edges()) {
    const auto& edge = edges_.at(ek);
    if (edge.has_vert(vk1)) {
      return ek;
    }
  }
  return EdgeKey{};
}

EdgeKey Mesh::add_edge(const EdgeVerts& verts) {
  current_edge_key_.advance();
  const auto ek = current_edge_key_;
  for (const auto vk : verts) {
    verts_.at(vk).add_edge(ek);
  }
  edges_.emplace(ek, verts);
  return ek;
}

EdgeKey Mesh::get_or_add_edge(const EdgeVerts& verts) {
  const EdgeKey ek = get_edge(verts);
  if (ek.valid()) {
    return ek;
  } else {
    return add_edge(verts);
  }
}

std::array<Vert*, 2> Mesh::get_edge_verts(EdgeKey ek) {
  const Edge& edge = edges_.at(ek);
  return {
      &verts_.at(edge.verts().at(0)),
      &verts_.at(edge.verts().at(1)),
  };
}

std::array<Vert*, 4> Mesh::get_quad_verts(QuadKey ek) {
  const Quad& quad = quads_.at(ek);
  return {
      &verts_.at(quad.verts().at(0)),
      &verts_.at(quad.verts().at(1)),
      &verts_.at(quad.verts().at(2)),
      &verts_.at(quad.verts().at(3)),
  };
}

QuadEdges Mesh::get_quad_edge_keys(const QuadKey qk) {
  const Quad& quad = quads_.at(qk);
  const auto vk0 = quad.verts().at(0);
  const auto vk1 = quad.verts().at(1);
  const auto vk2 = quad.verts().at(2);
  const auto vk3 = quad.verts().at(3);
  return {
      get_edge({vk0, vk1}),
      get_edge({vk1, vk2}),
      get_edge({vk2, vk3}),
      get_edge({vk3, vk0}),
  };
}

std::array<Edge*, 4> Mesh::get_quad_edges(const QuadKey qk) {
  const auto edges = get_quad_edge_keys(qk);
  return {
      &edges_.at(edges[0]),
      &edges_.at(edges[1]),
      &edges_.at(edges[2]),
      &edges_.at(edges[3]),
  };
}

VertKey Mesh::add_vert(const vec3& loc) {
  current_vert_key_.advance();
  const auto vk = current_vert_key_;
  verts_.emplace(vk, loc);
  return vk;
}

QuadKey Mesh::add_quad_from_verts(const QuadVerts& verts) {
  current_quad_key_.advance();
  const auto qk = current_quad_key_;
  for (std::size_t i = 0; i < verts.size(); i++) {
    const auto j = (i == 3) ? 0 : i + 1;
    const auto ek = get_or_add_edge({verts[i], verts[j]});
    auto& edge = edges_.at(ek);
    edge.add_quad(qk);
  }
  quads_.emplace(qk, verts);
  return qk;
}

void Mesh::remove_edge(EdgeKey ek) {
  const auto& edge = edges_.at(ek);
  // Adjacent quads are assumed to have already been removed
  assert(!edge.quads()[0].valid() &&
         !edge.quads()[1].valid());
  for (const auto& vk : edge.verts()) {
    auto& vert = verts_.at(vk);
    vert.remove_edge(ek);
  }
  edges_.erase(ek);
}

void Mesh::remove_quad(QuadKey qk) {
  auto edges = get_quad_edges(qk);
  for (auto& edge : edges) {
    edge->remove_quad(qk);
  }
  quads_.erase(qk);
}

void Mesh::subdivide() {
  //         e2
  //   v3___________ v2
  //    |           |
  //    |     |     |
  // e3 | ____c____ | e1
  //    |     |     |
  //    |     |     |
  //    |___________|
  //   v0            v1
  //         e0

  // TODO: for now select a single vertex at random. That vertices
  // edges will be subdivided.
  unordered_set<EdgeKey> edges;
  for (const auto ek : verts_.begin()->second.edges()) {
    edges.insert(ek);
  }

  // Subdivide edges
  unordered_map<EdgeKey, VertKey> edge_verts;
  unordered_set<QuadKey> quads;
  for (const auto ek : edges) {
    const auto verts = get_edge_verts(ek);
    const auto loc = average_vert_locs(verts);
    const auto vk = add_vert(loc);
    edge_verts.emplace(ek, vk);

    const Edge& edge = edges_.at(ek);
    for (const auto qk : edge.quads()) {
      if (qk.valid()) {
        quads.emplace(qk);
      }
    }
  }

  // Subdivide quads
  for (const auto qk : quads) {
    const auto verts = get_quad_verts(qk);
    const auto loc = average_vert_locs(verts);
    const auto vk = add_vert(loc);
    const auto quad_edge_keys = get_quad_edge_keys(qk);
    const auto quad_verts = quads_.at(qk).verts();

    // Delete old quad
    remove_quad(qk);

    for (int i = 0; i < 4; i++) {
      const int j = (i == 0) ? 3 : i - 1;
      const int k = (i == 3) ? 0 : i + 1;
      if (edges.count(quad_edge_keys[i]) &&
          edges.count(quad_edge_keys[j])) {
        add_quad_from_verts({
            vk,
            edge_verts[quad_edge_keys[j]],
            quad_verts[i],
            edge_verts[quad_edge_keys[i]]
        });
      } else if (edges.count(quad_edge_keys[j])) {
        add_quad_from_verts({
            vk,
            edge_verts[quad_edge_keys[j]],
            quad_verts[i],
            quad_verts[k],
        });
      } else if (edges.count(quad_edge_keys[i])) {
        add_quad_from_verts({
            vk,
            quad_verts[j],
            quad_verts[i],
            edge_verts[quad_edge_keys[i]],
        });
      }
    }
  }

  // Delete old edges
  for (const auto& ek : edges) {
    remove_edge(ek);
  }
}

void Mesh::validate() {
  check_for_loose_edges();
}

void Mesh::check_for_loose_edges() {
  for (const auto& iter : edges_) {
    const auto& edge = iter.second;
    assert(edge.quads()[0].valid() ||
           edge.quads()[1].valid());
  }
}
