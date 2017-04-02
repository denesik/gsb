layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec2 vert_uv;
layout(location = 2) in vec3 vert_normal;

uniform highp mat4 projectionMatrix;
uniform highp mat4 shadowMatrix;
uniform highp vec3 lightVector;

out vec2 frag_uv;
out vec3 frag_lightvector;
out mediump vec3 frag_normal;
out vec3 frag_shadow;

void main() {

	const float bias = 0.000005;

    frag_uv = vert_uv;

    gl_Position = projectionMatrix * vec4(vert_pos, 1);
	frag_shadow = (shadowMatrix * vec4(vert_pos, 1)).xyz;

	frag_lightvector = normalize(lightVector);

	frag_normal = vert_normal;
}
