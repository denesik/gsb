layout(location = 0) in vec4 vert_pos;

uniform highp mat4 projectionMatrix;

void main() {
  vec4 sc = projectionMatrix * vert_pos;
  //sc.x = (sc.x - 0.5f) / (abs(sc.x - 0.5f) + 0.5f) + 0.5f;
  //sc.y = (sc.y - 0.5f) / (abs(sc.y - 0.5f) + 0.5f) + 0.5f;
  gl_Position = sc;
}
