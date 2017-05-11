uniform sampler2D textureData;
uniform sampler2DShadow shadowDepth;

in vec2 frag_uv;

#if defined(GSB_SHADOWMAP_LGHT) || defined(GSB_NORMAL_LGHT)
in vec3 frag_lightvector;
in mediump vec3 frag_normal;
#endif

#ifdef GSB_SHADOWMAP_LGHT
in vec3 frag_shadow;
#endif

out vec4 frag_out;

float jitter[9] = float[9](0.001, -0.002, 0.0015, -0.0005, -0.0002, 0.0001, 0.00015, 0.00005, 0.000025);
vec2 shadow_texel_size = vec2( 1.0/2048.0, 1.0/2048.0 );
float shadow_bias = 0.00006;

float ShadowMap(vec2 loc, float pixel_z)
{
    return texture(shadowDepth, vec3(loc, pixel_z - shadow_bias));
}

float Gaussian(vec2 x, float a, vec2 b, float c)
{
    return a * exp( dot(x - b, x - b) / (2 * c * c) );
}

float ShadowMapPCF(vec2 center, float pixel_z, int pcf_size)
{
    vec2 base_loc = center - shadow_texel_size.xy * (pcf_size - 1) / 2.0f;
    vec2 c_loc = base_loc;
    float intensity = 0;
    float weight = 0;
    int c_jit = (int(c_loc.x) * 2048 * int(c_loc.y) * 2048) % 8;

    for(int i = 0; i < pcf_size; i++)
    {
        c_loc.x = base_loc.x;
        for(int j = 0; j < pcf_size; j++)
        {
            vec2 m_jit = vec2(jitter[c_jit], jitter[c_jit + 1]);
            c_jit = (c_jit + 2) % 8;

            float cwgt = Gaussian(c_loc + m_jit, 1, center, 1);
            intensity += ShadowMap(c_loc + m_jit, pixel_z) * cwgt;
            weight += cwgt;
            c_loc.x += shadow_texel_size.x;
        }
        c_loc.y += shadow_texel_size.y;
    }
    return intensity / weight;
}

void main() {
  vec4 albedo = texture(textureData, frag_uv);
  vec3 ambient = vec3(0.2, 0.2, 0.2);
  
  vec3 sc = frag_shadow;

  lowp float intensity;
  #ifdef GSB_NORMAL_LGHT
  intensity = dot(frag_normal, frag_lightvector);
  #else
  intensity = 1.0;
  #endif
  
  float inverseShadow;
  #ifdef GSB_SHADOWMAP_LGHT
  if(intensity <= 0)
  {
    inverseShadow = 0.0;
    intensity = 0.0;
  }
  else
  {
    inverseShadow = ShadowMapPCF(sc.xy, sc.z, 3);
  }
  #else
  inverseShadow = 1.0;
  #endif
  
  frag_out.rgb = ((ambient + vec3(intensity * inverseShadow))*albedo.rgb);
  frag_out.a = 1.0;
}
