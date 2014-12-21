//varying vec4 TexCoord;
uniform sampler2D tex0;
uniform float intensity;

void main ()
{
  vec4 pixel = texture2D(tex0, gl_TexCoord[0].xy)               * 1.0 +
               texture2D(tex0, gl_TexCoord[0].xy - intensity)   * 2.0 +
               texture2D(tex0, gl_TexCoord[0].xy + intensity)   * 2.0 +
               texture2D(tex0, gl_TexCoord[0].xy - intensity)   * 2.0 +
               texture2D(tex0, gl_TexCoord[0].xy + intensity)   * 2.0 +
               texture2D(tex0, gl_TexCoord[0].xy - 2*intensity) * 1.0 +
               texture2D(tex0, gl_TexCoord[0].xy)               * 1.0 +
               texture2D(tex0, gl_TexCoord[0].xy)               * 1.0 +
               texture2D(tex0, gl_TexCoord[0].xy)               * 1.0;

  gl_FragColor = pixel / 16.0;
}