﻿#ifndef Magnum_Examples_DebugLines_h
#define Magnum_Examples_DebugLines_h
/*
This file is part of Magnum.

Original authors — credit is appreciated but not required:

2010, 2011, 2012, 2013, 2014, 2015, 2016 —
Vladimír Vondruš <mosra@centrum.cz>
2016 — Bill Robinson <airbaggins@gmail.com>

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute
this software, either in source code form or as a compiled binary, for any
purpose, commercial or non-commercial, and by any means.

In jurisdictions that recognize copyright laws, the author or authors of
this software dedicate any and all copyright interest in the software to
the public domain. We make this dedication for the benefit of the public
at large and to the detriment of our heirs and successors. We intend this
dedication to be an overt act of relinquishment in perpetuity of all
present and future rights to this software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <Magnum/Buffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/SceneGraph/SceneGraph.h>
#include <Magnum/Shaders/VertexColor.h>

namespace Magnum {

  class DebugLines {
  public:
    typedef Shaders::VertexColor3D Shader;

    struct Point {
      Vector3 position;
      Color3 color;
    };

    explicit DebugLines();

    void reset();

    void addLine(const Point& p0, const Point& p1) {
      _lines.push_back(p0);
      _lines.push_back(p1);
    }

    void addLine(const Vector3& p0, const Vector3& p1, const Color3& col) {
      addLine({ p0, col }, { p1, col });
    }

    void addLine(const Vector3i& p0, const Vector3i& p1, const Color3& col) {
      addLine({ Vector3(p0), col }, { Vector3(p1), col });
    }

    void addLineDepth(const Point& p0, const Point& p1) {
      _linesDepth.push_back(p0);
      _linesDepth.push_back(p1);
    }

    void addLineDepth(const Vector3& p0, const Vector3& p1, const Color3& col) {
      addLineDepth({ p0, col }, { p1, col });
    }

    void addLineDepth(const Vector3i& p0, const Vector3i& p1, const Color3& col) {
      addLineDepth({ Vector3(p0), col }, { Vector3(p1), col });
    }

    void addFrustum(const Matrix4& imvp, const Color3& col);
    void addFrustum(const Matrix4& imvp, const Color3& col, Float z0, Float z1);

    void draw(const Matrix4& transformationProjectionMatrix);

  protected:
    std::vector<Point> _lines;
    Buffer _buffer;
    Mesh _mesh;
    Shader _shader;

    std::vector<Point> _linesDepth;
    Buffer _bufferDepth;
    Mesh _meshDepth;
  };

}

#endif