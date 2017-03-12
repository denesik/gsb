layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoordinates;
layout(location = 2) in vec3 normal;

uniform highp mat4 projectionMatrix;
uniform vec3 lightPos = vec3(1.0, 1, 1);

out vec2 interpolatedTextureCoordinates;
out vec3 lightvector;
out vec3 norm;

void main() {
    interpolatedTextureCoordinates = textureCoordinates;

    gl_Position = projectionMatrix * position;
    lightvector = normalize(lightPos);
    norm = normal;
}
