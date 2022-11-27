#include <QApplication>
#include <QtGui>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeComponent>
#include "backend.h"

int main(int argc, char *argv[])
{
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication app(argc, argv);

    BackEnd backEnd;    // Create the application core with signals and slots
    QDeclarativeView view;
    view.rootContext()->setContextProperty("backEnd", &backEnd);

    view.setSource(QUrl("qrc:/main.qml"));
    view.show();


//    QDeclarativeEngine engine;
//    QDeclarativeContext *context = new QDeclarativeContext(engine.rootContext());

//    /* We load the object into the context to establish the connection,
//     * and also define the name "appCore" by which the connection will occur
//     * */
//    context->setContextProperty("backEnd", &backEnd);
//    QDeclarativeComponent component(&engine, "qrc:/main.qml");
//    QObject *object = component.create(context);
//    const QUrl url(QStringLiteral("qrc:/main.qml"));
//    QObject::connect(&engine, &QDeclarativeEngine::objectCreated,
//                     &app, [url](QObject *obj, const QUrl &objUrl) {
//        if (!obj && url == objUrl)
//            QCoreApplication::exit(-1);
//    }, Qt::QueuedConnection);
//    engine.load(url);
    return app.exec();
}
