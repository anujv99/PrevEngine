#version 400 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec2 aInstancedOffset;

uniform mat4 projection;
uniform mat4 model;

uniform vec4 tileData;

out vec2 TextureCoord;

void main() {
	vec2 offsetPos = aPos + aInstancedOffset;
	gl_Position = projection * model * vec4(offsetPos, 0, 1);
	if (tileData.x != 0) {
		vec2 tileSize = vec2(1.0 / tileData.z, 1.0 / tileData.w);
		int tileY = int(tileData.y / tileData.z);
		int tileX = int(tileData.y) % int(tileData.z);
		TextureCoord = aTexCoord * tileSize + vec2(tileSize.x * tileX, tileSize.y * tileY);
	} else {
		TextureCoord = aTexCoord;
	}
}