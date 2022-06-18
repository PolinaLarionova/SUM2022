/**/
#version 450

layout(location = 0) out vec4 OutColor;

in vec4 DrawColor;
in vec3 DrawPos;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

uniform float Time;

uniform vec3 CamLoc;
uniform vec3 Ka, Kd, Ks;
uniform float Ph;

layout(binding = 0) uniform sampler2D Texture0;
uniform bool IsTexture0;

vec3 Shade( vec3 P, vec3 N, vec3 L, vec3 LColor )
{
  // Incident vector
  vec3 V = normalize(P - CamLoc);
  // Correct normal direction
  N = faceforward(N, V, N);

  // ambient
  vec3 color = Ka;

  // diffuse
  float nl = dot(N, L);
  if (nl > 0)
  {
    vec3 dif = Kd * nl * LColor;
    if (IsTexture0)
      dif *= texture(Texture0, DrawTexCoord).rgb;
    color += dif;

    // specular
    vec3 R = reflect(V, N);
    float rl = dot(R, L);
    if (rl > 0)
      color += LColor * Ks * pow(rl, Ph);
  }
  //return N;
  return color;
}

void main( void )
{
  vec3 N = normalize(DrawNormal);
  vec3 L = normalize(vec3(1 * sin(Time), 1, 1));
  if (!IsTexture0)
   OutColor = vec4(0.7, 0.7, 0.7, 1);
  else  
   OutColor = vec4(Shade(DrawPos, N, L, vec3(1, 1, 1)), 1);
}
