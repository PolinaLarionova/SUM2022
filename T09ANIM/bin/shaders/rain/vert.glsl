/**/
#version 450

layout(location = 0) in vec3 InPosition;

uniform mat4 MatrWVP, MatrW;

void main( void )
{
  gl_Position = vec4(InPosition, 1);
}
