layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoordinates;

uniform highp mat4 projectionMatrix;

out vec2 interpolatedTextureCoordinates;

void main() {
    interpolatedTextureCoordinates = textureCoordinates;

    gl_Position = projectionMatrix * position;
}
