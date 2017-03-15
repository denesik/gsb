uniform sampler2D textureData;

in vec2 interpolatedTextureCoordinates;

out vec4 fragmentColor;

void main() {
    fragmentColor.rgba = texture(textureData, interpolatedTextureCoordinates).rgba;
}
