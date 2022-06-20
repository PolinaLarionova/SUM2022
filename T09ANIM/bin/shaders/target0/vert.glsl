/**/
#version 450

out vec2 DrawTexCoord;

void main( void )
{
  struct pts
  {
    vec4 Pos;
    vec2 Tex;
  } p[4] =
  {
    {vec4(-1, 1, 0, 1), vec2 (0, 1)},
    {vec4(-1, -1, 0, 1), vec2 (0, 0)},
    {vec4(1, 1, 0, 1), vec2 (1, 1)},
    {vec4(1, -1, 0, 1), vec2 (1, 0)}
  };

  gl_Position = p[gl_VertexID].Pos;
  DrawTexCoord = p[gl_VertexID].Tex;
}
