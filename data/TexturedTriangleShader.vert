layout(location = 0) in vec4 vert_pos;
layout(location = 1) in vec2 vert_uv;
layout(location = 2) in vec3 vert_normal;

uniform highp mat4 projectionMatrix;
uniform highp mat4 shadowMatrix;
uniform highp vec3 lightVector;

out vec2 frag_uv;
out vec3 frag_lightvector;
out vec3 frag_normal;
out vec4 frag_shadow;

void main() {

	const float bias = 0.000005;

    frag_uv = vert_uv;

    gl_Position = projectionMatrix * vert_pos;
	frag_shadow = shadowMatrix * vec4(vert_pos.xyz, 1);

	frag_shadow.x  = frag_shadow.x * 0.5 + 0.5;
    frag_shadow.y  = frag_shadow.y * 0.5 + 0.5; 
    frag_shadow.z  = frag_shadow.z * 0.5 + 0.5 - bias;  

	frag_lightvector = normalize(lightVector);

	frag_normal = vert_normal;
}
