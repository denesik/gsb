layout(location = 0) in vec4 vert_pos;
layout(location = 1) in vec2 vert_uv;
layout(location = 2) in vec3 vert_normal;

uniform highp mat4 projectionMatrix;

out vec2 frag_uv;
out vec3 frag_lightvector;
out vec3 frag_normal;

void main() {
    frag_uv = vert_uv;

    gl_Position = projectionMatrix * vert_pos;

	frag_lightvector = normalize(vec3(100,1000,100));

	frag_normal = vert_normal;
}
