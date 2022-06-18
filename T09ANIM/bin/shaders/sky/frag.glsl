/**/
#version 450

layout(location = 0) out vec4 OutColor;

in vec4 DrawColor;
in vec3 DrawPos;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

uniform float Time;

layout(binding = 0) uniform sampler2D Texture0;
uniform bool IsTexture0;

void main( void )
{
  vec4 tc = texture(Texture0, DrawTexCoord);
  OutColor = tc.rgba;
}
