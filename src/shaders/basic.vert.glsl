uniform mat4 MVP;
attribute vec2 vPos;

void main() {
  gl_Position = MVP * vec4(vPos, 0.0, 1.0);
}
