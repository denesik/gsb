#include "StandartShader.h"

#include <Corrade/Utility/Resource.h>
#include <Magnum/Context.h>
#include <Magnum/Shader.h>
#include <Magnum/Version.h>


StandartShader::StandartShader() 
{
    MAGNUM_ASSERT_VERSION_SUPPORTED(Version::GL330);

    Shader vert{Version::GL330, Shader::Type::Vertex};
    Shader frag{Version::GL330, Shader::Type::Fragment};

    vert.addFile("data\\TexturedTriangleShader.vert");
    frag.addFile("data\\TexturedTriangleShader.frag");

    CORRADE_INTERNAL_ASSERT_OUTPUT(Shader::compile({vert, frag}));

    attachShaders({vert, frag});

    CORRADE_INTERNAL_ASSERT_OUTPUT(link());

    mUniformProjection = uniformLocation("projectionMatrix");
	mShadowProjection = uniformLocation("shadowMatrix");
	mLightVector = uniformLocation("lightVector");

    setUniform(uniformLocation("textureData"), TextureLayer);
	setUniform(uniformLocation("shadowDepth"), ShadowDepth);
}

ShadowShader::ShadowShader()
{
	MAGNUM_ASSERT_VERSION_SUPPORTED(Version::GL330);

	Shader vert{ Version::GL330, Shader::Type::Vertex };
	Shader frag{ Version::GL330, Shader::Type::Fragment };

	vert.addFile("data\\ShadowPass.vert");
	frag.addFile("data\\ShadowPass.frag");

	CORRADE_INTERNAL_ASSERT_OUTPUT(Shader::compile({ vert, frag }));

	attachShaders({ vert, frag });

	CORRADE_INTERNAL_ASSERT_OUTPUT(link());

	mUniformProjection = uniformLocation("projectionMatrix");
}

TexGenShader::TexGenShader()
{
	MAGNUM_ASSERT_VERSION_SUPPORTED(Version::GL330);

	Shader vert{ Version::GL330, Shader::Type::Vertex };
	Shader frag{ Version::GL330, Shader::Type::Fragment };

	vert.addFile("data\\Test.vert");
	frag.addFile("data\\Test.frag");

	CORRADE_INTERNAL_ASSERT_OUTPUT(Shader::compile({ vert, frag }));

	attachShaders({ vert, frag });

	CORRADE_INTERNAL_ASSERT_OUTPUT(link());

	setUniform(uniformLocation("inputTex0"), TextureLayer);
}
