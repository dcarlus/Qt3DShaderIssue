#ifndef VERTICALPLANEGEOMETRY_H
#define VERTICALPLANEGEOMETRY_H

#include <Qt3DCore>
#include <Qt3DRender>
#include <QtQuick>

class VerticalPlaneGeometry : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
    QML_NAMED_ELEMENT(VerticalPlaneGeometry)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged)

    private:
        static const int AmountVertices = 4;
        static const int AmountDataPerVertex = 3;

        ///
        /// \brief Width of the plane.
        ///
        int _width = 1;

        ///
        /// \brief Height of the plane.
        ///
        int _height = 1;

        ///
        /// \brief List of the vertices.
        ///
        QVector3D _vertices[AmountVertices];

        ///
        /// \brief Geometry shape that Qt3D is able to render.
        ///
        Qt3DCore::QGeometry* _geometry = nullptr;

        ///
        /// \brief Stores vertices data to be sent to shader programs.
        ///
        Qt3DCore::QBuffer* _vertexDataBuffer = nullptr;

        ///
        /// \brief Definition of how data should be read from a
        ///        @a vertexDataBuffer.
        ///
        Qt3DCore::QAttribute* _positionAttribute = nullptr;

    public:
        ///
        /// \brief Create a new VerticalPlaneGeometry instance.
        /// \param width
        /// \param height
        /// \param parent
        ///
        explicit VerticalPlaneGeometry(
            const int width = 1,
            const int height = 1,
            Qt3DCore::QNode* parent = nullptr
        );

        ///
        /// \brief Destruction of the VerticalPlaneGeometry instance.
        ///
        virtual ~VerticalPlaneGeometry() override;

        ///
        /// \brief Get the width of the plane.
        /// \return Width of the plane.
        ///
        int width() const { return _width; }

        ///
        /// \brief Set the width of the plane.
        /// \param width    Width of the plane.
        ///
        void setWidth(const int width);

        ///
        /// \brief Get the height of the plane.
        /// \return Height of the plane.
        ///
        int height() const { return _height; }

        ///
        /// \brief Set the height of the plane.
        /// \param height    Height of the plane.
        ///
        void setHeight(const int height);

        ///
        /// \brief Declare the class to the QML system.
        ///
        static void declareQML() {
            qmlRegisterType<VerticalPlaneGeometry>("test", 1, 0, "VerticalPlaneGeometry");
        }

    signals:
        ///
        /// \brief Signal when the width has changed.
        ///
        void widthChanged();

        ///
        /// \brief Signal when the height has changed.
        ///
        void heightChanged();

    private:
        ///
        /// \brief Generate the geometry data.
        ///
        void generateVertexBuffer();

        ///
        /// \brief Set the geometry attributes.
        ///
        void setAttributes();
};

#endif // VERTICALPLANEGEOMETRY_H
