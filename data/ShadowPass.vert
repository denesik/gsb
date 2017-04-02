layout(location = 0) in vec4 vert_pos;

uniform highp mat4 projectionMatrix;

void main() {
  vec4 sc = projectionMatrix * vert_pos;
  //sc.x = sc.x / (abs(sc.x) + 0.05);
  //sc.y = sc.y / (abs(sc.y) + 0.05);
  gl_Position = sc;
}
