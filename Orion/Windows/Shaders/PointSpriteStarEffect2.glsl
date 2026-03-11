#ifdef VERTEX_SHADER

uniform float near;
uniform float far;
uniform mat4 mvMatrix;
uniform mat4 pMatrix;
attribute vec4 vVertex;
attribute float vPointSize;
attribute vec4 vColor;
attribute float vIndex;
varying vec4 vVaryingColor;
uniform float relativeSize;
uniform float scale;
uniform float selectedIndex;

void main()
{
	mat4 mvpMatrix = pMatrix * mvMatrix;
    gl_Position = mvpMatrix * vVertex;
    gl_Position.z = 2.0 * log(gl_Position.w / near) / log(far / near) - 1.0;
    gl_Position.z *= gl_Position.w;

    if (selectedIndex >= 0.0 && selectedIndex == vIndex) {
        vVaryingColor = vec4(1.0, 0.0, 0.0, 1.0);
        gl_PointSize = (10.0 * relativeSize) / scale;
    }
    else {
        vVaryingColor = vColor;
        gl_PointSize = (vPointSize * relativeSize) / scale;
    }

    //gl_PointSize = relativeSize / scale;
}

#endif // VERTEX_SHADER

#ifdef FRAGMENT_SHADER
    
#ifdef OPENGL_ES
precision mediump float;
#endif

uniform sampler2D image;
varying vec4 vVaryingColor;
varying float pointsize;

void main()
{
    gl_FragColor = vec4(1.0, 1.0, 1.0, texture2D(image, gl_PointCoord)) * vVaryingColor;
}

#endif // FRAGMENT_SHADER
