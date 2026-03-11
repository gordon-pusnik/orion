#ifdef VERTEX_SHADER

uniform float near;
uniform float far;
uniform mat4 mvMatrix;
uniform mat4 pMatrix;
uniform vec3 vLightPos;
uniform vec4 vColor;
attribute vec4 vVertex;
attribute vec3 vNormal;
attribute vec2 vTexCoord0;

varying float Diffuse;
varying vec3 Specular;
varying vec2 TexCoord;

void main(void) 
{
    mat3 mNormalMatrix;

    mNormalMatrix[0] = normalize(mvMatrix[0].xyz);
    mNormalMatrix[1] = normalize(mvMatrix[1].xyz);
    mNormalMatrix[2] = normalize(mvMatrix[2].xyz);

    vec3 tnorm = normalize(mNormalMatrix * vNormal);
    vec4 ecPosition = mvMatrix * vVertex;
    vec3 ecPosition3;
    ecPosition3 = ecPosition.xyz / ecPosition.w;
    
    vec3 lightVec = normalize(vLightPos - ecPosition3);
    vec3 reflectVec = reflect(-lightVec, tnorm);
    vec3 viewVec    = normalize(-ecPosition3);

    float spec      = clamp(dot(reflectVec, viewVec), 0.0, 1.0);
    spec            = pow(spec, 8.0);
    Specular        = vec3 (spec) * vec3(1.0, 0.941, 0.898) * 0.3;

    Diffuse         = max(dot(lightVec, tnorm), 0.0);

    TexCoord = vTexCoord0;
    
    mat4 mvpMatrix = pMatrix * mvMatrix;
    gl_Position = mvpMatrix * vVertex;
    gl_Position.z = 2.0 * log(gl_Position.w / near) / log(far / near) - 1.0;
    gl_Position.z *= gl_Position.w;
}

#endif // VERTEX_SHADER

#ifdef FRAGMENT_SHADER

#ifdef OPENGL_ES
precision mediump float;
#endif

uniform sampler2D textureUnit1;
uniform sampler2D textureUnit2;
uniform sampler2D textureUnit3;

varying float Diffuse;
varying vec3 Specular;
varying vec2 TexCoord;

void main(void)
{
    // Monochrome cloud cover value will be in clouds.r
    // Gloss value will be in clouds.g
    // clouds.b will be unused

    vec2 clouds    = texture2D(textureUnit3, TexCoord).rg;
    vec3 daytime   = (texture2D(textureUnit1, TexCoord).rgb * Diffuse + 
                          Specular * clouds.g) * (1.0 - clouds.r) +
                          clouds.r * Diffuse;
    vec3 nighttime = texture2D(textureUnit2, TexCoord).rgb * 
                         (1.0 - clouds.r) * 2.0;

    vec3 color = daytime;

    if (Diffuse <= 0.1)
        color = mix(nighttime, daytime, (Diffuse + 0.1) * 5.0);

    gl_FragColor = vec4(color, 1.0);
}

#endif // FRAGMENT_SHADER

