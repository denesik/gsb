#pragma once

#include <Magnum/Texture.h>
#include <Magnum/Mesh.h>
#include <Magnum/Framebuffer.h>
#include <Magnum/Buffer.h>
#include <Magnum/Defaultframebuffer.h>
#include <StandartShader.h>

using namespace Magnum;

class TextureGenerator
{
public:
	TextureGenerator(TexGenShader & shader, const Vector2i &size);

	Texture2D &Generate(Texture2D & source);

private:
	Texture2D mTexture;
	Framebuffer mFbo;
	DefaultFramebuffer mFbo0;
	TexGenShader & mShader;

	Mesh m;
	Buffer vertex_buffer;
	Buffer index_buffer;
};

