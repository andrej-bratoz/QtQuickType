#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QQmlContext>
#include <qqml.h>
#include <Windows.h>
#include "quicktype.h"

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString command READ command WRITE setCommand NOTIFY onCommandChanged)
    Q_PROPERTY(QString currentProcess READ currentProcess WRITE setCurrentProcess NOTIFY onCurrentProcessChanged)
    Q_PROPERTY(QList<QString> options READ options NOTIFY onOptionsChanged)
	Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY onOptionSelected)
    QML_ELEMENT

public:
    explicit BackEnd(QObject* parent = nullptr);
    //
    QString command() const;
    QString currentProcess() const;
    QList<QString> options() const;
    int selectedIndex() const;
	
	//
    void setCommand(const QString& command);
    void setCurrentProcess(const QString& process);
    void setSelectedIndex(int index);
	//

    void clearCommandList();

    void handleWindowsEventHookCallback(HWINEVENTHOOK hWinEventHook,
        uint eventType, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread,
        DWORD dwmsEventTime);
		

	
  
signals:
    void onOptionsChanged();
    void onCommandChanged();
    void onCurrentProcessChanged();
    void onOptionSelected(int index);
	

private:
    WINEVENTPROC m_proc;
    QString m_command;
    QString m_currentProcess;
    QList<QString> m_options;
    int m_selectedIndex;
};


#endif // BACKEND_H
