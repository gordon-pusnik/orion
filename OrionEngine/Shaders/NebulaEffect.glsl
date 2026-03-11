#ifdef VERTEX_SHADER

uniform float near;
uniform float far;
uniform mat4 mvMatrix;
uniform mat4 pMatrix;
uniform vec3  LightPosition;
attribute vec4 vVertex;
attribute vec3 vNormal;
attribute vec2 vTexCoord0;
varying vec2 vTex;

varying float LightIntensity;
varying vec3 MCposition;

uniform float relativeSize;
uniform float scale;

void main()
{
	mat4 mvpMatrix = pMatrix * mvMatrix;
    gl_Position = mvpMatrix * vVertex;
    gl_Position.z = 2.0 * log(gl_Position.w / near) / log(far / near) - 1.0;
    gl_Position.z *= gl_Position.w;
    
    gl_PointSize = relativeSize / scale;
}

#endif // VERTEX_SHADER

#ifdef FRAGMENT_SHADER

#ifdef OPENGL_ES
precision mediump float;
#endif

uniform sampler2D Noise;
uniform vec4 Color1;       // (0.8, 0.7, 0.0)
uniform vec4 Color2;       // (0.6, 0.1, 0.0)
uniform float NoiseScale;  // 1.2

varying float LightIntensity;
varying vec3 MCposition;

uniform vec3 Offset;

varying vec2 vTex;

void main()
{
    gl_FragColor = texture2D(Noise, gl_PointCoord) * Color1;
}

#endif // FRAGMENT_SHADER
