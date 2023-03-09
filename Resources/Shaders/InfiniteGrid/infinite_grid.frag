// Code adapted to Qt from:
// http://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/

#version 450

// Matrices.
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
// Camera parameters.
uniform float camNearPlane;
uniform float camFarPlane;
uniform vec3 camUp;
uniform vec3 camPosition;
// Grid parameters.
uniform float globalAlpha;
uniform vec3 smallestGridColor;
uniform vec3 intermediateGridColor;
uniform vec3 biggestGridColor;


in vec3 nearPoint;
in vec3 farPoint;

out vec4 fragColor;


// Use the scale variable to set the distance between the lines.
vec4 grid(vec3 fragPos3D, float scale, float lineWidth, vec3 lineColor) {
    vec2 coord = fragPos3D.xz / scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5f) - 0.5f) / derivative;
    float line = min(grid.x, grid.y) / lineWidth;
    float minimumz = min(derivative.y, 1.f);
    float minimumx = min(derivative.x, 1.f);

    vec4 color = vec4(lineColor, 1.0 - min(line, 1.f));

    float limit = 0.1f * lineWidth ;

    // z axis
    if (fragPos3D.x > -limit * minimumx && fragPos3D.x < limit * minimumx) {
        color.xyz = vec3(0.1f, 0.2f, 1.f);
    }

    // x axis
    if (fragPos3D.z > -limit * minimumz && fragPos3D.z < limit * minimumz) {
        color.xyz = vec3(1.f, 0.2f, 0.1f);
    }

    return color;
}

float computeDepth(vec3 pos) {
    vec4 clipSpacePos = projectionMatrix * viewMatrix * vec4(pos.xyz, 1.f);
    float clipSpaceDepth = clipSpacePos.z / clipSpacePos.w;
    return (clipSpaceDepth + 1.f) * 0.5f;
}

float computeLinearDepth(vec3 pos) {
    float near = camNearPlane;
    float far = camFarPlane;

    vec4 clipSpacePos = projectionMatrix * viewMatrix * vec4(pos.xyz, 1.f);
    float clipSpaceDepth = (clipSpacePos.z / clipSpacePos.w) * 2.f - 1.f;
    float linearDepth = (2.f * near * far) / (far + near - clipSpaceDepth * (far - near));
    return linearDepth / far; // normalize
}

void main() {
    const vec3 NormalGridPlane = vec3(0., 1., 0.);
    float dotGridCam = 1.f - dot(abs(camUp), NormalGridPlane);

    const float MaxAlphaDistance = 1.f;
    float eyeGridDistance = abs(camPosition.y);
    float gridAlpha = 1.f;

    if (eyeGridDistance < MaxAlphaDistance) {
        gridAlpha = max(abs(dotGridCam), eyeGridDistance);
    }

    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    float linearDepth = computeLinearDepth(fragPos3D);
    float fading = max(0.f, (0.5f - linearDepth));

    gl_FragDepth = computeDepth(fragPos3D);

    vec3 gridColor = smallestGridColor;//0.05f
    float smallestGridFading = linearDepth * 200.f;
    vec4 smallestGrid = grid(fragPos3D, 0.1f, 1.f, gridColor);
    smallestGrid.a *= min(1.f, max(0.f, 1.f - smallestGridFading)) * float(gl_FragDepth < 1.f) * gridAlpha;

    gridColor = intermediateGridColor;//0.1f
    float smallGridFading = linearDepth * 30.f;
    vec4 smallGrid = grid(fragPos3D, 1.f, 1.f, gridColor);
    smallGrid.a *= min(1.f, max(0.f, 1.f - smallGridFading)) * float(gl_FragDepth < 1.f) * gridAlpha * 1.f;

    gridColor = biggestGridColor;//0.15f
    float bigGridFading = linearDepth * 15.f;
    vec4 bigGrid = grid(fragPos3D, 10.f, 1.f, gridColor);
    bigGrid.a *= min(1.f, max(0.f, 1.f - bigGridFading)) * float(gl_FragDepth < 1.f) * gridAlpha * 2.f;

    fragColor = smallestGrid + smallGrid + bigGrid;
    fragColor.a *= globalAlpha;
}
