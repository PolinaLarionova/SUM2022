/**/
#version 420

layout(location = 0) out vec4 OutColor;

in vec4 DrawColor;
in vec3 DrawPos;
in vec2 DrawTexCoord;

layout(binding = 0) uniform sampler2D Texture0;

void main( void )
{
  vec4 tc = texture(Texture0, DrawTexCoord);
  if (tc.a < 0.5)
   discard;
  OutColor = tc.rgba;
}
