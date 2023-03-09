#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "VerticalPlaneGeometry.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QQuickWindow::setTextRenderType(QQuickWindow::TextRenderType::NativeTextRendering);

    // Set OpenGL requirements
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setVersion(4, 6);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);

    VerticalPlaneGeometry::declareQML();

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/Qt3DShaderIssue/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
