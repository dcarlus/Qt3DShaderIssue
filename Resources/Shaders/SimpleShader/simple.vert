#version 450 core

uniform mat4 modelViewProjection;

layout(location = 0) in vec4 vertexPosition;

void main() {
    vec3 pos = vertexPosition.xyz;
    gl_Position = modelViewProjection * vec4(pos, 1.f);
}
