#ifdef VERTEX_SHADER

uniform float near;
uniform float far;
uniform mat4 mvMatrix;
uniform mat4 pMatrix;
attribute vec4 vVertex;

void main()
{
	mat4 mvpMatrix = pMatrix * mvMatrix;
    gl_Position = mvpMatrix * vVertex;

    gl_Position.z = 2.0 * log(gl_Position.w / near) / log(far / near) - 1.0;
    gl_Position.z *= gl_Position.w;

    gl_PointSize = 1.0;
}

#endif // VERTEX_SHADER

#ifdef FRAGMENT_SHADER
    
#ifdef OPENGL_ES
precision mediump float;
#endif

uniform vec4 vColor;

void main()
{
    gl_FragColor = vColor;
}

#endif // FRAGMENT_SHADER
