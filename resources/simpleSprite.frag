
#ifdef GL_ES
varying lowp vec4 v_VertexColor;
varying mediump vec4 v_TexCoord;
#else
varying vec4 v_VertexColor;
varying vec4 v_TexCoord;
#endif

uniform sampler2D u_SpriteTexture;

void main()
{
  vec4 texel = texture2D(u_SpriteTexture, v_TexCoord.st);
	gl_FragColor = texel;//v_VertexColor;
}