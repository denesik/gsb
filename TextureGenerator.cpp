#include "TextureGenerator.h"
#include <Magnum\TextureFormat.h>
#include <Magnum\PixelFormat.h>
#include <Magnum\Image.h>
#include <Corrade\Utility\Assert.h>

TextureGenerator::TextureGenerator(TexGenShader & shader, const Vector2i &size) : mShader(shader), mFbo({ {}, size })
{
	m.setPrimitive(MeshPrimitive::Triangles);
	m.addVertexBuffer(vertex_buffer, 0, TexGenShader::Position{});
	m.setIndexBuffer(index_buffer, 0, Mesh::IndexType::UnsignedByte);

	vertex_buffer.setData(std::vector<Vector3>{{-1, -1, 0}, {1, -1, 0}, {-1, 1, 0}, {1, 1, 0}}, BufferUsage::StaticDraw);
	index_buffer.setData(std::vector<unsigned char>{0, 1, 2, 1, 3, 2}, BufferUsage::StaticDraw);
	m.setCount(2);

	ImageView2D image{ PixelFormat::RGBA, PixelType::UnsignedByte,{ 512, 512 }, nullptr };
	mTexture.setMagnificationFilter(Sampler::Filter::Linear)
		.setMinificationFilter(Sampler::Filter::Linear)
		.setStorage(0, TextureFormat::RGBA, image.size())
		.setImage(0, TextureFormat::RGBA, image);

	mFbo.attachTexture(Framebuffer::ColorAttachment(0), mTexture, 0).mapForDraw(Framebuffer::ColorAttachment(0));

	CORRADE_INTERNAL_ASSERT(mFbo.checkStatus(FramebufferTarget::Draw) == Framebuffer::Status::Complete);
}

Texture2D & TextureGenerator::Generate(Texture2D & source)
{
	mFbo.clear(FramebufferClear::Color).bind();
	mShader.setTexture(source);
	m.draw(mShader);
	mFbo0.bind();

	return mTexture;
}
