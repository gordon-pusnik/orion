#ifdef VERTEX_SHADER

uniform float near;
uniform float far;
uniform mat4 mvMatrix;
uniform mat4 pMatrix;
uniform float type;

attribute vec3 initialVelocity;
attribute float startTime;

uniform vec3 emitterPos;
uniform vec3 velocity;
uniform float time;
varying float opacity;
varying float render;

vec3 fountain(float t)
{
    opacity = 1.0 - (t / 3.0) - 0.2;
    
    vec3 p;
    
    if (t > 0.0) {
        vec3 a = vec3(0.0, -10.0, 0.0); // gravity
        p = emitterPos + initialVelocity * t + 0.5 * a * t * t;
        render = 1.0;
    }
    else {
        p = emitterPos;
        render = 0.0;
    }
    
    gl_PointSize = 10.0;

    return p;
}

vec3 pulse(float t)
{
    opacity = 1.0 - (t / 10.0) - 0.4;
    float size = 25.0 - (t / 2.0);

    vec3 p;

    if (t > 0.0) {
        p = emitterPos + velocity * t;
        render = 1.0;
    }
    else {
        p = emitterPos;
        render = 0.0;
    }
    
    gl_PointSize = size;

    return p;
}

void main()
{
    float t = time - startTime;
        
    vec3 p;
    
    if (type == 0.0) {
        p = fountain(t);
    }
    else if (type > 0.0) {
        p = pulse(t);
    }

	mat4 mvpMatrix = pMatrix * mvMatrix;
    gl_Position = mvpMatrix * vec4(p, 1.0);
    gl_Position.z = 2.0 * log(gl_Position.w / near) / log(far / near) - 1.0;
    gl_Position.z *= gl_Position.w;
}

#endif // VERTEX_SHADER

#ifdef FRAGMENT_SHADER
    
#ifdef OPENGL_ES
precision mediump float;
#endif

uniform sampler2D image;
varying float opacity;
varying float render;
uniform vec4 color1;
uniform vec4 color2;

void main()
{
    if (render > 0.0) {
        vec4 texcol = texture2D(image, gl_PointCoord);
        gl_FragColor = vec4(color1.r * (color2.r - opacity),
                            color1.g * (color2.g - opacity),
                            color1.b * (color2.b - opacity),
                            color1.a * (1.0)) * texcol;
    }
    else {
        discard;
    }
}

#endif // FRAGMENT_SHADER
