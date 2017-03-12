uniform vec3 color = vec3(1.0, 1.0, 1.0);
uniform sampler2D textureData;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);

in vec2 interpolatedTextureCoordinates;
in vec3 lightvector;
in vec3 norm;

out vec4 fragmentColor;

void main() {
    vec3 ambient = color * texture(textureData, interpolatedTextureCoordinates).rgb;

    float coef = min(1, max(0, dot(norm, lightvector)) + 0.4);

    fragmentColor = vec4(ambient * coef, 1.0);
}
