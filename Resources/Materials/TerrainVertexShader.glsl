#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 worldTransform;
uniform mat4 cameraTransform;
uniform mat4 projectionMatrix;

out vec3 world_normal;

void main()
{
    gl_Position = projectionMatrix * inverse(cameraTransform) * vec4(pos, 1);
    world_normal = vec3(0, 0, 1);
}
