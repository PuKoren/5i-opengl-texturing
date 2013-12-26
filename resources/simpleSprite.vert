
attribute vec4 a_Position;
attribute vec4 a_TexCoord;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelViewMatrix;

uniform vec3 u_DiffuseColor;

varying vec4 v_TexCoord;
varying vec4 v_VertexColor;

void main()
{
	v_TexCoord = a_TexCoord;
	v_VertexColor = vec4(u_DiffuseColor, 1.0);
	gl_Position = u_ProjectionMatrix * u_ModelViewMatrix * a_Position;
}