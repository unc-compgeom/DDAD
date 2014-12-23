/*
 * This file is part of DDAD.
 *
 * DDAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * DDAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with DDAD. If not, see <http://www.gnu.org/licenses/>.
 */

// DDAD
#include "common.h"
#include "opengl.h"

namespace DDAD {

//=============================================================================
// ShaderManager
//=============================================================================

/*
ShaderManager::ShaderManager() {
    initializeOpenGLFunctions();

    // override system locale until shaders are compiled
    setlocale(LC_NUMERIC, "C");

    auto AddProgram = [&](Visual::Material::Coverage coverage,
                          Visual::Material::Lighting lighting,
                          const QString& vpath, const QString& fpath) {
        QSharedPointer<QOpenGLShaderProgram> sp(new QOpenGLShaderProgram());
        bool v = sp->addShaderFromSourceFile(QOpenGLShader::Vertex, vpath);
        bool f = sp->addShaderFromSourceFile(QOpenGLShader::Fragment, fpath);
        bool l = sp->link();

        if (v && f && l) {
            programs_.insert(id, sp);
            return true;
        } else {
            qDebug() << id;
            return false;
        }
    };

    bool shaders_valid =
               AddProgram(Visual::Material::MC_OPAQUE,
                          Visual::Material::ML_UNLIT,
                          ":/shaders/mat_unlit_opaque.vsh",
                          ":/shaders/mat_unlit_opaque.fsh")
            && AddProgram(Visual::Material::MC_OPAQUE,
                          Visual::Material::ML_FLAT,
                          ":/shaders/mat_flat_opaque.vsh",
                          ":/shaders/mat_flat_opaque.fsh")
            && AddProgram(Visual::Material::MC_TRANSPARENT,
                          Visual::Material::ML_UNLIT,
                          ":/shaders/mat_unlit_transparent.vsh",
                          ":/shaders/mat_unlit_transparent.fsh")
            && AddProgram(Visual::Material::MC_TRANSPARENT,
                          Visual::Material::ML_FLAT,
                          ":/shaders/mat_flat_transparent.vsh",
                          ":/shaders/mat_flat_transparent.fsh");

    if (!shaders_valid) {
        qDebug() << "Shaders did not compile.";
    }

    // restore system locale
    setlocale(LC_ALL, "");
}
*/

/*
bool ShaderManager::addProgram(const QString& id, const QString& vert_path,
                               const QString &frag_path) {
    QSharedPointer<QOpenGLShaderProgram> sp(new QOpenGLShaderProgram());
    bool v = sp->addShaderFromSourceFile(QOpenGLShader::Vertex, vert_path);
    bool f = sp->addShaderFromSourceFile(QOpenGLShader::Fragment, frag_path);
    bool l = sp->link();

    if (v && f && l) {
        programs_.insert(id, sp);
        return true;
    } else {
        qDebug() << id;
        return false;
    }
}

QSharedPointer<QOpenGLShaderProgram> ShaderManager::getProgram(
    const QString& id) {
    return programs_.value(id);
}
*/

//=============================================================================
// OpenGL abstraction layer
//=============================================================================

namespace GL {

void EnableAttributes(QSharedPointer<QOpenGLShaderProgram> program,
                      const QList<AttributeMeta>& attributes) {
    foreach (AttributeMeta attr_meta, attributes) {
        program->enableAttributeArray(attr_meta.name);
        program->setAttributeBuffer(attr_meta.name,
                                    attr_meta.type, attr_meta.offset,
                                    attr_meta.count, Vertex::kStride);
    }
}

void DisableAttributes(QSharedPointer<QOpenGLShaderProgram> program,
                       const QList<AttributeMeta>& attributes) {
    foreach (AttributeMeta attr_meta, attributes) {
        program->disableAttributeArray(attr_meta.name);
    }
}

//=============================================================================
// AttributeMeta
//=============================================================================

AttributeMeta::AttributeMeta() {}

AttributeMeta::AttributeMeta(const int count, const GLenum type,
                             const int offset, const char* name) :
    count(count),
    type(type),
    offset(offset),
    name(name) {}

//=============================================================================
// Vertex
//=============================================================================

// static vertex attribute definitions

const AttributeMeta Vertex::kPositionMeta(3, GL_FLOAT,
                        offsetof(Vertex, position_), "v_position");
const AttributeMeta Vertex::kTangentMeta(3, GL_FLOAT,
                        offsetof(Vertex, tangent_), "v_tangent");
const AttributeMeta Vertex::kNormalMeta(3, GL_FLOAT,
                        offsetof(Vertex, normal_), "v_normal");
const AttributeMeta Vertex::kBitangentMeta(3, GL_FLOAT,
                        offsetof(Vertex, bitangent_), "v_bitangent");
const AttributeMeta Vertex::kUvcoordsMeta(2, GL_FLOAT,
                        offsetof(Vertex, uvcoords_), "v_multi_tex_coord_0");
const AttributeMeta Vertex::kMatAmbientMeta(4, GL_FLOAT,
                        offsetof(Vertex, mat_ambient_), "v_mat_ambient");
const GLsizei Vertex::kStride = static_cast<GLsizei>(sizeof(Vertex));

// constructors

Vertex::Vertex() {}

Vertex::Vertex(const Point_3f &position, const Point_2f &uvcoords) {
    set_position(position);
    set_uvcoords(uvcoords);
}

Vertex::Vertex(const Point_3f &position, const QColor &mat_ambient) {
    set_position(position);
    set_mat_ambient(mat_ambient);
}

Vertex::Vertex(const Point_3f &position, const Vector_3f &normal) {
    set_position(position);
    set_normal(normal);
}

Vertex::Vertex(const Point_3f &position, const Vector_3f &normal,
               const QColor &mat_ambient) {
    set_position(position);
    set_normal(normal);
    set_mat_ambient(mat_ambient);
}

Vertex::Vertex(const Point_3f &position, const Vector_3f &tangent,
               const Vector_3f &normal, const Vector_3f &bitangent,
               const Point_2f &uvcoords, const QColor &mat_ambient) {
    set_position(position);
    set_tangent(tangent);
    set_normal(normal);
    set_bitangent(bitangent);
    set_uvcoords(uvcoords);
    set_mat_ambient(mat_ambient);
}

// setters

void Vertex::set_position(const Point_3f& position) {
    position_ = position.elements();
}
void Vertex::set_tangent(const Vector_3f& tangent) {
    tangent_ = tangent.elements();
}
void Vertex::set_normal(const Vector_3f& normal) {
    normal_ = normal.elements();
}
void Vertex::set_bitangent(const Vector_3f& bitangent) {
    bitangent_ = bitangent.elements();
}
void Vertex::set_uvcoords(const Point_2f& uvcoords) {
    uvcoords_ = uvcoords.elements();
}
void Vertex::set_mat_ambient(const QColor& mat_ambient) {
    mat_ambient_[0] = mat_ambient.redF();
    mat_ambient_[1] = mat_ambient.greenF();
    mat_ambient_[2] = mat_ambient.blueF();
    mat_ambient_[3] = mat_ambient.alphaF();
}
void Vertex::set_mat_ambient(const Visual::Color& mat_ambient) {
    mat_ambient_[0] = static_cast<GLfloat>(mat_ambient.r())/255.0f;
    mat_ambient_[1] = static_cast<GLfloat>(mat_ambient.g())/255.0f;
    mat_ambient_[2] = static_cast<GLfloat>(mat_ambient.b())/255.0f;
    mat_ambient_[3] = static_cast<GLfloat>(mat_ambient.a())/255.0f;
}

//=============================================================================
// VertexBuffer
//=============================================================================

VertexBuffer::VertexBuffer() :
    num_vertices(0) {}

void VertexBuffer::UploadVertices(const QVector<Vertex> &vertices) {
    if(!buffer.isCreated()) {
        buffer.create();
        buffer.bind();
        buffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    }
    buffer.bind();
    buffer.allocate(vertices.data(), vertices.size()*sizeof(Vertex));
    buffer.release();

    num_vertices = vertices.size();
}

//=============================================================================
// ElementArray
//=============================================================================

ElementArray::ElementArray() {}

ElementArray::ElementArray(const QString &tag, const GLenum mode,
                           const GLsizei count, const GLenum type,
                           GLvoid * const indices) :
    tag(tag),
    mode(mode),
    count(count),
    type(type),
    indices(indices) {}


} // namespace GL

} // namespace DDAD
