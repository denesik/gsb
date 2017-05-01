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

void main() {
  vec4 albedo = texture(textureData, frag_uv);
  vec3 ambient = vec3(0.2, 0.2, 0.2);
  
  vec3 sc = frag_shadow;
  sc.x = sc.x / (abs(sc.x) + 0.005);
  sc.y = sc.y / (abs(sc.y) + 0.005);

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
    inverseShadow = texture( shadowDepth, vec3(sc.xy, sc.z - 0.0005));
  }
  #else
  inverseShadow = 1.0;
  #endif
  
  frag_out.rgb = ((ambient + vec3(intensity * inverseShadow))*albedo.rgb);
  frag_out.a = 1.0;
}
