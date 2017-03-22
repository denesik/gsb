uniform sampler2D textureData;

in vec2 frag_uv;
in vec3 frag_lightvector;
in vec3 frag_normal;

out vec4 frag_out;

void main() {
    vec4 tcol = texture(textureData, frag_uv);

	float coef = min(1, max(0, dot(frag_normal, frag_lightvector)) + 0.4);

	frag_out = vec4((coef * tcol).rgb, 1);
}
