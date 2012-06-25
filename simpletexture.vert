

varying float Diffuse;
varying vec3  Specular;
varying vec2  TexCoord;
varying vec2  TexCoord2;

uniform vec3 LightPosition;

void main(void)
{
    vec3 ecPosition = vec3 (gl_ModelViewMatrix * gl_Vertex);
    vec3 tnorm      = normalize(gl_NormalMatrix * gl_Normal);
    vec3 lightVec   = normalize(LightPosition - ecPosition);
    vec3 reflectVec = reflect(-lightVec, tnorm);
    vec3 viewVec    = normalize(-ecPosition);

    float spec      = clamp(dot(reflectVec, viewVec), 0.0, 1.0);
    spec            = pow(spec, 80.0);
    Specular        = vec3 (spec) * vec3 (1.0, 0.941, 0.898) * 0.3;

    Diffuse         = max(dot(lightVec, tnorm), 0.0);
	
    TexCoord        = gl_MultiTexCoord0.st;
    TexCoord2       = gl_MultiTexCoord0.st * 3.0;
    gl_Position     = ftransform();
}