/**/
#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 MatrWVP, MatrW;
uniform vec3 CamLoc, CamRight, CamUp;

out vec2 DrawTexCoord;
//out float h;

void main( void )
{
  vec3
    p = gl_in[0].gl_Position.xyz,
    r = CamRight,
    u = CamUp;

  //h = (MatrW * vec4(p, 1)).y;

  gl_Position = MatrWVP * vec4(p - r + u, 1);
  DrawTexCoord = vec2(0, 1); 
  EmitVertex();

  gl_Position = MatrWVP * vec4(p - r - u, 1);
  DrawTexCoord = vec2(0, 0);
  EmitVertex();

  gl_Position = MatrWVP * vec4(p + r + u, 1);
  DrawTexCoord = vec2(1, 1); 
  EmitVertex();

  gl_Position = MatrWVP * vec4(p + r - u, 1);
  //gl_Position = vec4(-1, -1, 0, 1);
  DrawTexCoord = vec2(1, 0); 
  EmitVertex();
  EndPrimitive();

  /*



  EndPrimitive();

  gl_Position = vec4(-1, -1, 0, 1);
  DrawTexCoord = vec2(0, 0);
  EmitVertex();

  gl_Position = vec4(1, 1, 0, 1);
  DrawTexCoord = vec2(1, 1); 
  EmitVertex();
  */

}
