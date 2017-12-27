#ifndef SRC_SHADER_HH_
#define SRC_SHADER_HH_

#include <string>

#include <epoxy/gl.h>

#include "common.hh"

namespace mgrid {

class Shader {
 public:
  static Shader create(GLenum kind, const std::string& contents);

  Shader(Shader&& other);
  Shader(Shader&) = delete;
  ~Shader();

  Shader& operator=(Shader&& other);

  void attach(GLuint program);

 private:
  Shader(GLenum kind, GLuint handle);

  GLenum kind_;
  GLuint handle_;
};

class ShaderProgram {
 public:
  ShaderProgram();
  ShaderProgram(ShaderProgram&&);
  ShaderProgram(ShaderProgram&) = delete;

  ~ShaderProgram();

  ShaderProgram& operator=(ShaderProgram&& other);

  void create_frag_shader(const std::string& code);
  void create_geom_shader(const std::string& code);
  void create_tess_ctrl_shader(const std::string& code);
  void create_tess_eval_shader(const std::string& code);
  void create_vert_shader(const std::string& code);

  void link();

  GLint attribute_location(const std::string& name);
  GLint uniform_location(const std::string& name);

  void bind();

 private:
  GLuint handle_;
  optional<Shader> frag_;
  optional<Shader> geom_;
  optional<Shader> tess_ctrl_;
  optional<Shader> tess_eval_;
  optional<Shader> vert_;
};
}

#endif  // SRC_SHADER_HH_
