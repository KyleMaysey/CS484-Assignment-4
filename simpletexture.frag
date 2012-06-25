
uniform sampler2D IsotropicTexture;
uniform sampler2D AnisotropicTexture;


varying float Diffuse;
varying vec3  Specular;
varying vec2  TexCoord;
varying vec2  TexCoord2;

void main (void)
{
    
    /*vec3 isotropic = (texture2D(IsotropicTexture, TexCoord).rgb * Diffuse + 
                          Specular) * Diffuse;
    vec3 anisotropic = texture2D(AnisotropicTexture, TexCoord).rgb;*/
    
    vec3 isotropic = texture2D( IsotropicTexture, TexCoord2).rgb;
    vec3 anisotropic = texture2D( AnisotropicTexture, TexCoord ).rgb;
    
    vec3 color = mix(anisotropic, isotropic, (Diffuse + 0.1 * 3.0) );
    //vec3 color = anisotropic - isotropic;
    gl_FragColor = vec4 (color, 1.0);
}