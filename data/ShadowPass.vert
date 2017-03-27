layout(location = 0) in vec4 vert_pos;

uniform highp mat4 projectionMatrix;

void main() {
    gl_Position = projectionMatrix * vert_pos;
}
