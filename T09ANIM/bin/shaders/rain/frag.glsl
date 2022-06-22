/**/
#version 450

layout(location = 0) out vec4 OutColor;
layout(location = 1) out vec4 Out1;

in vec2 DrawTexCoord; 
//in float h;

layout(binding = 0) uniform sampler2D Texture0;
uniform bool IsTexture0;
                             
void main( void )
{             
  vec4 tc = texture(Texture0, DrawTexCoord);
  //if (tc.a < 0.30)
  //  discard;
  //float s = h < 0.30 ? h / 0.30 : 1;
  //OutColor = vec4(tc.rgb, tc.a * s);
  OutColor = tc.rgba;            
  Out1 = vec4(1, 1, 1, 1);             
}
