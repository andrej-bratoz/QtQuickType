#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStringListModel>
#include <memory>
#include <QDir>

#include "../inc/qt_objects.h"
#include "../inc/quicktype.h"
#include "../inc/win_hooks.h"

BackEnd backend;

int main(int argc, char* argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QDir::setCurrent(qApp->applicationDirPath());
    const Index index("index.xml");
    if (index.IsValid())
    {
	    CommandList list(index);
	    SetWindowsEventHook_ForegroundWindow(WindowsEventHook_CallBack);
		backend.RegisterCmdIndex(&list);
	    engine.rootContext()->setContextProperty("backEnd", &backend);
		
	    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
	    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
	        &app, [url](QObject* obj, const QUrl& objUrl) {
	            if (!obj && url == objUrl)
	                QCoreApplication::exit(-1);
	        }, Qt::QueuedConnection);
	    engine.load(url);
	    return app.exec();
    }
	else
	{
		MessageBox(nullptr, L"Cannot find file 'index.xml' in root folder. Application will exit.", L"Critical file missing", MB_OK | MB_ICONERROR);
	}

}
