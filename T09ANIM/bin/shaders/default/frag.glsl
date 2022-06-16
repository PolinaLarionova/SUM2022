/**/
#version 330

layout(location = 0) out vec4 OutColor;

in vec4 DrawColor;
in vec3 DrawPos;

uniform float Time;

void main( void )
{
  OutColor = DrawColor;
}
