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

    gl_PointSize = 25.0;
}

#endif // VERTEX_SHADER

#ifdef FRAGMENT_SHADER
    
#ifdef OPENGL_ES
precision mediump float;
#endif

uniform sampler2D textureUnitPoint;
uniform sampler2D textureUnitBracket;
uniform vec4 vColor;
uniform float selected;

void main()
{
    if (selected > 0.0) {
        gl_FragColor = texture2D(textureUnitBracket, gl_PointCoord) * vColor;
    }
    else {
        gl_FragColor = texture2D(textureUnitPoint, gl_PointCoord) * vColor;
    }
}

#endif // FRAGMENT_SHADER
