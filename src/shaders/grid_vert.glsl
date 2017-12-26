#version 330

in vec3 vertexPosition;

uniform mat4 modelView;
uniform mat3 modelViewNormal;
uniform mat4 modelViewProjection;

void main()
{
  //position = vec3( modelView * vec4( vertexPosition, 1.0 ) );

    gl_Position = modelViewProjection * vec4( vertexPosition, 1.0 );
}
