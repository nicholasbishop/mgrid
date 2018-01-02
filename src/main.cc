#include <iostream>

#include "external/json.hpp"
#include "src/mesh.hh"

using json = nlohmann::json;


int main() {
  auto mesh = Mesh::cube();
  mesh.subdivide();

  const json obj{
    {"verts", mesh.verts()},
    {"faces", mesh.quads()}
  };

  mesh.validate();

  std::cout << obj << std::endl;
}
