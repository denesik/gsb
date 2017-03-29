uniform sampler2D textureData;
uniform sampler2DShadow shadowDepth;

in vec2 frag_uv;
in vec3 frag_lightvector;
in vec3 frag_normal;
in vec3 frag_shadow;

out vec4 frag_out;

void main() {
    vec4 tcol = texture(textureData, frag_uv);

	float coef = min(1, max(0, dot(frag_normal, frag_lightvector)) + 0.4);

	float visibility = 1.0;
    float inv = texture( shadowDepth, vec3(frag_shadow.xy, frag_shadow.z));
    visibility = visibility * inv;

	frag_out = vec4(frag_shadow.z/100, 0, 0, 1);
}
