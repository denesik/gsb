layout(location = 0) in vec4 vert_pos;

uniform highp mat4 projectionMatrix;

void main() {
  vec4 sc = projectionMatrix * vert_pos;
  gl_Position = sc;
}
