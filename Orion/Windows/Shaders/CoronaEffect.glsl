#ifdef VERTEX_SHADER

uniform float near;
uniform float far;
uniform mat4 mvMatrix;
uniform mat4 pMatrix;
uniform float fTime;
attribute vec4 vVertex;
attribute vec4 vColor;
varying vec4 vVaryingColor;

void main()
{
	mat4 mvpMatrix = pMatrix * mvMatrix;
/*    
    float scale = 4.0;
    float x = vVertex.x;
    float y = vVertex.y;

    // calculate a scale factor.
    float s = sin((fTime + 3.0 * y) * scale);
    float c = cos((fTime + 5.0 * x) * scale);
    float z = 0.05 * s * c;

    // offset the position along the normal.
    vec3 v = vVertex.xyz + sin(z);

    gl_Position = mvpMatrix * vec4(v, 1.0);
*/
    gl_Position = mvpMatrix * vVertex;
    
    gl_Position.z = 2.0 * log(gl_Position.w / near) / log(far / near) - 1.0;
    gl_Position.z *= gl_Position.w;

    vVaryingColor = vColor;
}

#endif // VERTEX_SHADER

#ifdef FRAGMENT_SHADER
    
#ifdef OPENGL_ES
precision mediump float;
#endif

varying vec4 vVaryingColor;

void main()
{
    gl_FragColor = vVaryingColor;
}

#endif // FRAGMENT_SHADER
