#include "VerticalPlaneGeometry.h"

VerticalPlaneGeometry::VerticalPlaneGeometry(
    const int width,
    const int height,
    Qt3DCore::QNode* parent
) : QGeometryRenderer(parent),
    _width(width),
    _height(height),
    _geometry(new Qt3DCore::QGeometry(this)),
    _vertexDataBuffer(new Qt3DCore::QBuffer(_geometry))
{
    generateVertexBuffer();
    setAttributes();

    setPrimitiveType(Qt3DRender::QGeometryRenderer::TriangleStrip);
    setGeometry(_geometry);
    setVertexCount(AmountVertices);
    setInstanceCount(1);
    setIndexOffset(0);
    setFirstInstance(0);
}

VerticalPlaneGeometry::~VerticalPlaneGeometry()
{
    _geometry -> deleteLater();
    _positionAttribute -> deleteLater();
    _vertexDataBuffer -> deleteLater();
}

void VerticalPlaneGeometry::setWidth(const int width)
{
    _width = width;
    generateVertexBuffer();
    emit widthChanged();
}

void VerticalPlaneGeometry::setHeight(const int height)
{
    _height = height;
    generateVertexBuffer();
    emit heightChanged();
}

void VerticalPlaneGeometry::generateVertexBuffer()
{
    const float X = _width / 2.f;
    const float Y = _height / 2.f;

    // Fill the vertex buffer.
    _vertices[0] = QVector3D(-X, -Y, 0.f);
    _vertices[1] = QVector3D(-X, +Y, 0.f);
    _vertices[2] = QVector3D(+X, -Y, 0.f);
    _vertices[3] = QVector3D(+X, +Y, 0.f);

    const int Stride = 3 * sizeof(float);
    const int BufferSize = AmountVertices * AmountDataPerVertex * Stride;
    QByteArray verticesBytes(BufferSize, Qt::Initialization::Uninitialized);
    float* geometryBytes = reinterpret_cast<float*>(verticesBytes.data());

    for (QVector3D& vertex : _vertices)
    {
        *geometryBytes++ = vertex.x();
        *geometryBytes++ = vertex.y();
        *geometryBytes++ = vertex.z();
    }

    _vertexDataBuffer -> setData(verticesBytes);
}

void VerticalPlaneGeometry::setAttributes()
{
    const int ByteOffset = 0 * sizeof(float);

    _positionAttribute = new Qt3DCore::QAttribute();
    _positionAttribute -> setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    _positionAttribute -> setBuffer(_vertexDataBuffer);
    _positionAttribute -> setVertexBaseType(Qt3DCore::QAttribute::Float);
    _positionAttribute -> setVertexSize(AmountDataPerVertex);
    _positionAttribute -> setByteOffset(ByteOffset);
    _positionAttribute -> setByteStride(AmountDataPerVertex * sizeof(float));
    _positionAttribute -> setCount(AmountVertices);
    _positionAttribute -> setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    _geometry -> addAttribute(_positionAttribute);
}
