/**/
#version 450

layout(location = 0) out vec4 OutColor;

in vec2 DrawTexCoord;

layout(binding = 0) uniform sampler2D Texture0;
layout(binding = 1) uniform sampler2D Texture1;

layout(binding = 8) uniform sampler2D ShadowMap;

uniform float Time;

void main( void )
{
  ivec2 sc = ivec2(gl_FragCoord.xy);
  vec4 tc = texelFetch(Texture0, sc, 0);
  vec3 A[9];

 // for (int i = 0; i < 3; i++)
  //  for (int j = 0; j < 3; j++)
   //   A[i * 3 + j] = texelFetch(Texture0, sc + ivec2(j - 1, i - 1), 0);

  OutColor = tc.rgba;
}
