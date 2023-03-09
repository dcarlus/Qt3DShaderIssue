// Code adapted to Qt from:
// http://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/

#version 450

uniform mat4 inverseViewMatrix;
uniform mat4 inverseProjectionMatrix;
uniform mat4 modelViewProjection;

in vec4 vertexPosition;

out vec3 nearPoint;
out vec3 farPoint;

vec3 unprojectPoint(float x, float y, float z) {
    vec4 unprojectedPoint =  inverseViewMatrix * inverseProjectionMatrix * vec4(x, y, z, 1.f);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 pos = vertexPosition.xyz;
    nearPoint = unprojectPoint(pos.x, pos.y, 0.f);
    farPoint = unprojectPoint(pos.x, pos.y, 1.f);
    gl_Position = vec4(pos, 1.f);
}
