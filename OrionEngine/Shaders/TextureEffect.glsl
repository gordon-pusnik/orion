#ifdef VERTEX_SHADER

uniform float near;
uniform float far;
uniform mat4 mvMatrix;
uniform mat4 pMatrix;
attribute vec4 vVertex;
attribute vec2 vTexCoord0;
varying vec2 vTex;

void main(void) 
{
    vTex = vTexCoord0;
    mat4 mvpMatrix;
    mvpMatrix = pMatrix * mvMatrix;
    gl_Position = mvpMatrix * vVertex;
    gl_Position.z = 2.0 * log(gl_Position.w / near) / log(far / near) - 1.0;
    gl_Position.z *= gl_Position.w;
}

#endif // VERTEX_SHADER

#ifdef FRAGMENT_SHADER

#ifdef OPENGL_ES
precision mediump float;
#endif

varying vec2 vTex;
uniform sampler2D textureUnit;

void main(void) 
{
    gl_FragColor = texture2D(textureUnit, vTex);
}

#endif // FRAGMENT_SHADER
