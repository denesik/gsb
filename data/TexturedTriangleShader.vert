layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec2 vert_uv;
layout(location = 2) in vec3 vert_normal;

uniform highp mat4 projectionMatrix;
uniform highp mat4 shadowMatrix;
uniform highp vec3 lightVector;

out vec2 frag_uv;

#if defined(GSB_SHADOWMAP_LGHT) || defined(GSB_NORMAL_LGHT)
out vec3 frag_lightvector;
out mediump vec3 frag_normal;
#endif

#ifdef GSB_SHADOWMAP_LGHT
out vec3 frag_shadow;
#endif

void main() {
  frag_uv = vert_uv;
  
  gl_Position = projectionMatrix * vec4(vert_pos, 1);

  #ifdef GSB_SHADOWMAP_LGHT
  frag_shadow = (shadowMatrix * vec4(vert_pos, 1)).xyz;
  #endif
  
  #if defined(GSB_SHADOWMAP_LGHT) || defined(GSB_NORMAL_LGHT)
  frag_lightvector = normalize(lightVector);
  frag_normal = vert_normal;
  #endif
}
