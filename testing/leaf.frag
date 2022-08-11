uniform sampler2D Tex;

void main (void)
{
   vec4 color = texture2D(Tex, gl_TexCoord[0].st);

   if (color.r > 0.8 && color.g > 0.8 && color.b > 0.8)
      discard;
   else;

   gl_FragColor = color * gl_Color * 0.85;
}
