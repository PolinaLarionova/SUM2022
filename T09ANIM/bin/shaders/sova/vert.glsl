/**/
#version 450

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

uniform mat4 MatrWVP;
uniform mat4 MatrW;
uniform mat4 MatrWInv;
uniform float Time;

out vec4 DrawColor;
out vec3 DrawPos;
out vec3 DrawNormal;
out vec2 DrawTexCoord;

void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition, 1);
  DrawColor = InColor;
  DrawPos = (MatrW * vec4(InPosition, 1)).xyz;
  DrawNormal = mat3(MatrWInv) * InNormal;
  DrawTexCoord = InTexCoord;
}
