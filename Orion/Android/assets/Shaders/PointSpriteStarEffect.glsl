#ifdef VERTEX_SHADER

uniform float near;
uniform float far;
uniform mat4 mvMatrix;
uniform mat4 pMatrix;
attribute vec4 vVertex;
attribute vec4 vColor;
varying vec4 vVaryingColor;

void main()
{
	mat4 mvpMatrix = pMatrix * mvMatrix;
    gl_Position = mvpMatrix * vVertex;
    gl_Position.z = 2.0 * log(gl_Position.w / near) / log(far / near) - 1.0;
    gl_Position.z *= gl_Position.w;

    vVaryingColor = vColor;

    float size = 1.0;

    if (vColor.a < 1.0 && vColor.a > 0.95) {
        size = 3.0;
    }
    else if (vColor.a <= 0.95 && vColor.a > 0.9) {
        size = 2.0;
    }
    else if (vColor.a <= 0.9 && vColor.a > 0.8) {
        size = 1.0;
    }
    else if (vColor.a <= 0.8 && vColor.a > 0.7) {
        size = 1.0;
    }
    else if (vColor.a < 0.7) {
        size = 1.0;
    }

    gl_PointSize = size;
}

#endif // VERTEX_SHADER

#ifdef FRAGMENT_SHADER
    
#ifdef OPENGL_ES
precision mediump float;
#endif

uniform sampler2D image;
varying vec4 vVaryingColor;

void main()
{
    gl_FragColor = texture2D(image, gl_PointCoord) * vVaryingColor;
}

#endif // FRAGMENT_SHADER
