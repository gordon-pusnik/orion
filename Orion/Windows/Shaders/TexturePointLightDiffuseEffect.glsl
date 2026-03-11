#ifdef VERTEX_SHADER

uniform float near;
uniform float far;
uniform mat4 mvMatrix;
uniform mat4 pMatrix;
uniform vec3 vLightPos;
uniform vec4 vColor;
attribute vec4 vVertex;
attribute vec3 vNormal;
varying vec4 vFragColor;
attribute vec2 vTexCoord0;
varying vec2 vTex;
uniform float minimum;

#ifndef OPENGL_ES
varying float logz;
#endif

void main(void) 
{
    mat3 mNormalMatrix;

    mNormalMatrix[0] = normalize(mvMatrix[0].xyz);
    mNormalMatrix[1] = normalize(mvMatrix[1].xyz);
    mNormalMatrix[2] = normalize(mvMatrix[2].xyz);

    vec3 vNorm = normalize(mNormalMatrix * vNormal);
    vec4 ecPosition;
    vec3 ecPosition3;
    
    ecPosition = mvMatrix * vVertex;
    ecPosition3 = ecPosition.xyz / ecPosition.w;
    
    vec3 vLightDir = normalize(vLightPos - ecPosition3);
    float fDot = max(minimum, dot(vNorm, vLightDir));
    
    vFragColor.rgb = vColor.rgb * fDot;
    vFragColor.a = vColor.a;
    
    vTex = vTexCoord0;
    
    mat4 mvpMatrix = pMatrix * mvMatrix;
    
    gl_Position = mvpMatrix * vVertex;

//#ifdef OPENGL_ES
//    float C = 10000.0;
//    gl_Position.z = 2.0 * log(gl_Position.w * C + 1.0) / log(far * C + 1.0) - 1.0;

    gl_Position.z = 2.0 * log(gl_Position.w / near) / log(far / near) - 1.0;
    gl_Position.z *= gl_Position.w;
/*    
#else
    float C = 100.0;
    float FC = 1.0 / log(far * C + 1.0);
    logz = log(gl_Position.w * C + 1.0) * FC;
    gl_Position.z = (2.0 * logz - 1.0) * gl_Position.w;
#endif
*/
}

#endif // VERTEX_SHADER

#ifdef FRAGMENT_SHADER

#ifdef OPENGL_ES
precision mediump float;
#else
varying float logz;
#endif

varying vec4 vFragColor;
varying vec2 vTex;
uniform sampler2D textureUnit;

void main(void)
{
    gl_FragColor = vFragColor * texture2D(textureUnit, vTex);
    
#ifndef OPENGL_ES
//    gl_FragDepth = logz;
#endif
}

#endif // FRAGMENT_SHADER

