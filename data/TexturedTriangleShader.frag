uniform sampler2D textureData;
uniform sampler2D shadowDepth;

in vec2 frag_uv;
in vec3 frag_lightvector;
in vec3 frag_normal;
in vec4 frag_shadow;

out vec4 frag_out;

void main() {
    vec4 tcol = texture(textureData, frag_uv);

	float coef = min(1, max(0, dot(frag_normal, frag_lightvector)) + 0.4);
	float shadow = 1;

	if (texture(shadowDepth, frag_shadow.xy).x  <  frag_shadow.z)
		shadow -= 1;

	frag_out = vec4((coef * tcol).rgb, 1);
}
