#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QQmlContext>
#include <qqml.h>
#include "quicktype.h"

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString command READ command WRITE setCommand NOTIFY onCommandChanged)
    Q_PROPERTY(QString currentProcess READ currentProcess WRITE setCurrentProcess NOTIFY onCurrentProcessChanged)
    Q_PROPERTY(QList<QString> options READ options NOTIFY onOptionsChanged)
    QML_ELEMENT

public:
    explicit BackEnd(QObject* parent = nullptr);
    //
    QString command() const;
    QString currentProcess() const;
    QList<QString> options() const;
	
	//
    void setCommand(const QString& command);
    void setCurrentProcess(const QString& process);

	//
  
signals:
    void onOptionsChanged();
    void onCommandChanged();
    void onCurrentProcessChanged();
private:
    QString m_command;
    QString m_currentProcess;
    QList<QString> m_options;
};


#endif // BACKEND_H
