#version 330 core

// <vec2 position, vec2 texCoords>
layout (location = 0) in vec4 vertex;

out vec2 uv;

uniform bool shake;
uniform float time;
uniform float scale;

void main() {
	
    gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
    uv = vertex.zw;

    if (shake) 
	{
        float strength = 0.01f * scale;
        gl_Position.x += cos(time * 10) * strength;
        gl_Position.y += cos(time * 15) * strength;
    }
}
