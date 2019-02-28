#version 400 core

out vec4 FragColor;

uniform sampler2D firstTexture;

in vec2 TextureCoord;

void main() {
	FragColor = vec4(1, 1, 0, 1);
	//FragColor = texture(firstTexture, TextureCoord);
	if (FragColor.a < 1.0) {
		discard;
	}
}