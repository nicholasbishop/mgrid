#version 400
	 
layout(quads) in;
in vec3 tcPosition[];
out vec3 tePosition;
out vec4 tePatchDistance;

uniform mat4 modelViewProjection;

void main()
{
  float u = gl_TessCoord.x, v = gl_TessCoord.y;
  vec3 a = mix(tcPosition[0], tcPosition[1], u);
  vec3 b = mix(tcPosition[3], tcPosition[2], u);
  tePosition = mix(a, b, v);
  tePosition.z = pow(u, 2) * 5 + pow(v, 2) * 5;
  tePatchDistance = vec4(u, v, 1-u, 1-v);
  gl_Position = modelViewProjection * vec4(tePosition, 1);
}
