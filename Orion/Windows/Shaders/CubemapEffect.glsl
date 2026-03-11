#ifdef VERTEX_SHADER

uniform mat4 mvMatrix;
uniform mat4 pMatrix;
attribute vec4 vVertex;
varying vec3 vVaryingTexCoord;

void main()
{
    vVaryingTexCoord = normalize(vVertex.xyz);
    mat4 mvpMatrix = pMatrix * mvMatrix;
    gl_Position = mvpMatrix * vVertex;
}

#endif

#ifdef FRAGMENT_SHADER

#ifdef OPENGL_ES
precision mediump float;
#endif

uniform samplerCube cubemap;
varying vec3 vVaryingTexCoord;

void main()
{
    gl_FragColor = textureCube(cubemap, vVaryingTexCoord);
}

#endif
