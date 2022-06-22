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

mat4 rotz( float a )
{
  a = a * acos(-1) / 180;
  float c = cos(a), s = sin(a);

  return mat4(vec4(c, s, 0, 0),
              vec4(-s, c, 0, 0),
              vec4(0, 0, 1, 0),
              vec4(0, 0, 0, 1));
}
               
void main( void )
{
  float s = 5;
  vec3 v = vec3(0, 0, 0);
  if (InPosition.y >= 0 && InPosition.y < 0.351001)
  {
    float shift = InPosition.x > 0 ? acos(-1) / 1.0 : 1;

    v.z = 0.050 * sin(Time * s + shift);
    v.y = 0.028 * clamp(sin(Time * s + shift + 1), 0, 1);
  }
  gl_Position = MatrWVP * rotz(s * sin(Time * 5)) * vec4(InPosition + v, 1);
  //gl_Position = MatrWVP * vec4(InPosition, 1);
  DrawColor = InColor;
  DrawPos = (MatrW * vec4(InPosition, 1)).xyz;
  DrawNormal = mat3(MatrWInv) * InNormal;
  DrawTexCoord = InTexCoord;
}
