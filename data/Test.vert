layout(location = 0) in vec4 vert_pos;

out vec2 frag_uv;

void main() {
    gl_Position = vert_pos;
	frag_uv = vert_pos.xy * 0.5 + vec2(0.5, 0.5);
}
