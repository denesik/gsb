#include "StandartShader.h"

#include <Corrade/Utility/Resource.h>
#include <Magnum/Context.h>
#include <Magnum/Shader.h>
#include <Magnum/Version.h>


StandartShader::StandartShader() 
{
    MAGNUM_ASSERT_VERSION_SUPPORTED(Version::GL330);

    //const Utility::Resource rs{"textured-triangle-data"};

    Shader vert{Version::GL330, Shader::Type::Vertex};
    Shader frag{Version::GL330, Shader::Type::Fragment};

//     vert.addSource(rs.get("TexturedTriangleShader.vert"));
//     frag.addSource(rs.get("TexturedTriangleShader.frag"));
    vert.addFile("data\\TexturedTriangleShader.vert");
    frag.addFile("data\\TexturedTriangleShader.frag");

    CORRADE_INTERNAL_ASSERT_OUTPUT(Shader::compile({vert, frag}));

    attachShaders({vert, frag});

    CORRADE_INTERNAL_ASSERT_OUTPUT(link());

    _colorUniform = uniformLocation("color");
    mUniformProjection = uniformLocation("projectionMatrix");

    setUniform(uniformLocation("textureData"), TextureLayer);
}

