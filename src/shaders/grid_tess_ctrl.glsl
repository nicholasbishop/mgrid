#version 400

layout(vertices = 4) out;
in vec3 vPosition[];
out vec3 tcPosition[];

const int inner = 128;
const int outer = 128;
	 
#define ID gl_InvocationID
	 
void main()
{
  tcPosition[ID] = vPosition[ID];
  if (ID == 0) {
    gl_TessLevelInner[0] = inner;
    gl_TessLevelInner[1] = inner;
    gl_TessLevelOuter[0] = outer;
    gl_TessLevelOuter[1] = outer;
    gl_TessLevelOuter[2] = outer;
    gl_TessLevelOuter[3] = outer;
  }
}
