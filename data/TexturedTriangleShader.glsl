uniform sampler2D textureData;
uniform sampler2DArrayShadow shadowmapTexture;

uniform highp mat4 projectionMatrix;
uniform highp mat4 shadowmapMatrix[NUM_SHADOW_MAP_LEVELS];
uniform highp vec3 lightVector;

uniform float shadowDepthSplits[NUM_SHADOW_MAP_LEVELS];

#ifdef _VERTEX_

layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec2 vert_uv;
layout(location = 2) in vec3 vert_normal;

//////////////////////////////////

out highp vec2 frag_uv;

#if defined(GSB_SHADOWMAP_LGHT) || defined(GSB_NORMAL_LGHT)
out vec3 frag_lightvector;
out mediump vec3 frag_normal;
#endif

#ifdef GSB_SHADOWMAP_LGHT
out highp vec3 frag_shadow[NUM_SHADOW_MAP_LEVELS];
#endif

///////////////////////////////////

void main() {
  frag_uv = vert_uv;
  
  vec4 pos4 = projectionMatrix * vec4(vert_pos, 1);
  gl_Position = pos4;

  #ifdef GSB_SHADOWMAP_LGHT
  for(int i = 0; i < shadowmapMatrix.length(); i++) {
    frag_shadow[i] = (shadowmapMatrix[i] * pos4).xyz;
  }
  #endif
  
  #if defined(GSB_SHADOWMAP_LGHT) || defined(GSB_NORMAL_LGHT)
  frag_lightvector = normalize(lightVector);
  frag_normal = vert_normal;
  #endif
}

#endif // _VERTEX_

///////////////////////////////////

#ifdef _FRAGMENT_

///////////////////////////////////

in vec2 frag_uv;

#if defined(GSB_SHADOWMAP_LGHT) || defined(GSB_NORMAL_LGHT)
in vec3 frag_lightvector;
in mediump vec3 frag_normal;
#endif

#ifdef GSB_SHADOWMAP_LGHT
in vec3 frag_shadow[NUM_SHADOW_MAP_LEVELS];
#endif

///////////////////////////////////

out vec4 frag_out;

///////////////////////////////////

float jitter[9] = float[9](0.001, -0.002, 0.0015, -0.0005, -0.0002, 0.0001, 0.00015, 0.00005, 0.000025);
vec2 shadow_texel_size = vec2( 1.0/2048.0, 1.0/2048.0 );
float shadow_bias = 0.00006;

float ShadowMap(vec2 loc, float pixel_z)
{
    return texture(shadowmapTexture, vec4(loc, 0, pixel_z - shadow_bias));
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
  vec4 textureSample = texture(textureData, frag_uv);
  vec3 albedo = textureSample.rgb;
  vec3 ambient = vec3(0.2, 0.2, 0.2);
  
  vec3 sc = frag_shadow[0];

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
	int shadowLevel = 0;
    bool inRange = false;

    for(; shadowLevel < NUM_SHADOW_MAP_LEVELS; ++shadowLevel) {
        vec3 shadowCoord = frag_shadow[shadowLevel];
        inRange = shadowCoord.x >= 0 &&
                  shadowCoord.y >= 0 &&
                  shadowCoord.x <  1 &&
                  shadowCoord.y <  1 &&
                  shadowCoord.z >= 0 &&
                  shadowCoord.z <  1;
        if(inRange) {
            inverseShadow = ShadowMap(shadowCoord.xy, shadowCoord.z);//  texture(shadowmapTexture, vec4(shadowCoord.xy, shadowLevel, shadowCoord.z - shadow_bias));
            break;
        }
    }

    #ifdef DEBUG_SHADOWMAP_LEVELS
    switch(shadowLevel) {
        case 0: albedo *= vec3(1,0,0); break;
        case 1: albedo *= vec3(1,1,0); break;
        case 2: albedo *= vec3(0,1,0); break;
        case 3: albedo *= vec3(0,1,1); break;
        default: albedo *= vec3(1,0,1); break;
    }
    #else
    if(!inRange) {
        albedo *= vec3(1,0,1); //Something has gone wrong - didn't find a shadow map
    }
    #endif
  
    //inverseShadow = ShadowMapPCF(sc.xy, sc.z, 3);
  }
  #else
  inverseShadow = 1.0;
  #endif
  
  frag_out = vec4((ambient + vec3(intensity * inverseShadow)) * albedo, 1.0);
}

#endif // _FRAGMENT_
