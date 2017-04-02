uniform sampler2D textureData;
uniform sampler2DShadow shadowDepth;

in vec2 frag_uv;
in vec3 frag_lightvector;
in mediump vec3 frag_normal;
in vec3 frag_shadow;

out vec4 frag_out;

void main() {
    vec4 albedo = texture(textureData, frag_uv);
    vec3 ambient = vec3(0.2,0.2,0.2);

    lowp float intensity = dot(frag_normal, frag_lightvector);

    float inverseShadow = 1.0;

    if(intensity <= 0)
    {
        inverseShadow = 0.0f;
        intensity = 0.0f;
    }
    else
    {
        inverseShadow = texture( shadowDepth, vec3(frag_shadow.xy, frag_shadow.z - 0.005));
    }

    frag_out.rgb = ((ambient + vec3(intensity * inverseShadow))*albedo.rgb);
    frag_out.a = 1.0;
}
