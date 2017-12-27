#include "shader.hh"

#include <iostream>
#include <stdexcept>
#include <string>

#include "errors.hh"

namespace mgrid {

class ShaderError : public std::runtime_error {
public:
  ShaderError(const std::string &what) : runtime_error(what) {}
};

Shader Shader::create(const GLenum kind, const std::string &contents) {
  const char *c_str = contents.c_str();
  const int count = 1;

  const auto shader = glCreateShader(kind);
  if (!shader) {
    throw ShaderError("glCreateShader failed");
  }

  glShaderSource(shader, count, &c_str, NULL);
  glCompileShader(shader);

  GLint log_length = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
  std::string log;
  log.resize(log_length);
  glGetShaderInfoLog(shader, log_length, &log_length, &log[0]);
  log.resize(log_length);

  if (!log.empty()) {
    std::cout << log << std::endl;
  }

  return Shader{kind, shader};
}

Shader::Shader(Shader &&other) { std::swap(handle_, other.handle_); }

Shader::Shader(GLenum kind, GLuint handle) : kind_(kind), handle_(handle) {
  if (!handle_) {
    throw ShaderError("Shader: invalid handle");
  }
}

Shader::~Shader() { glDeleteShader(handle_); }

Shader &Shader::operator=(Shader &&other) {
  std::swap(handle_, other.handle_);
  return *this;
}

void Shader::attach(GLuint program) {
  if (handle_) {
    glAttachShader(program, handle_);
  }
}

ShaderProgram::ShaderProgram() : handle_(glCreateProgram()) {
  if (!handle_) {
    throw ShaderError("ShaderProgram: invalid handle");
  }
}

ShaderProgram::ShaderProgram(ShaderProgram &&other) {
  std::swap(handle_, other.handle_);
  std::swap(frag_, other.frag_);
  std::swap(geom_, other.geom_);
  std::swap(tess_ctrl_, other.tess_ctrl_);
  std::swap(tess_eval_, other.tess_eval_);
  std::swap(vert_, other.vert_);
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(handle_); }

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&other) {
  std::swap(handle_, other.handle_);
  std::swap(frag_, other.frag_);
  std::swap(geom_, other.geom_);
  std::swap(tess_ctrl_, other.tess_ctrl_);
  std::swap(tess_eval_, other.tess_eval_);
  std::swap(vert_, other.vert_);
  return *this;
}

void ShaderProgram::create_frag_shader(const std::string &code) {
  frag_ = std::move(Shader::create(GL_FRAGMENT_SHADER, code));
}

void ShaderProgram::create_geom_shader(const std::string &code) {
  geom_ = Shader::create(GL_GEOMETRY_SHADER, code);
}

void ShaderProgram::create_tess_ctrl_shader(const std::string &code) {
  tess_ctrl_ = Shader::create(GL_TESS_CONTROL_SHADER, code);
}

void ShaderProgram::create_tess_eval_shader(const std::string &code) {
  tess_eval_ = Shader::create(GL_TESS_EVALUATION_SHADER, code);
}

void ShaderProgram::create_vert_shader(const std::string &code) {
  vert_ = Shader::create(GL_VERTEX_SHADER, code);
}

void ShaderProgram::link() {
  frag_->attach(handle_);
  geom_->attach(handle_);
  tess_ctrl_->attach(handle_);
  tess_eval_->attach(handle_);
  vert_->attach(handle_);

  check_gl_error("pre-link");
  glLinkProgram(handle_);
  check_gl_error("post-link");
}

GLint ShaderProgram::uniform_location(const std::string &name) {
  bind();
  const auto loc = glGetUniformLocation(handle_, name.c_str());
  if (loc == -1) {
    throw ShaderError("glGetUniformLocation failed");
  }
  return loc;
}

GLint ShaderProgram::attribute_location(const std::string &name) {
  bind();
  const auto loc = glGetAttribLocation(handle_, name.c_str());
  if (loc == -1) {
    throw ShaderError("glGetAttribLocation failed");
  }
  return loc;
}

void ShaderProgram::bind() { glUseProgram(handle_); }
}
