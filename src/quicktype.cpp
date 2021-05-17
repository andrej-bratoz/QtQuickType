#include "quicktype.h"

BackEnd::BackEnd(QObject* parent) : QObject(parent), m_command("")
{
}

QString BackEnd::command() const{
    return m_command;
}

void BackEnd::setCommand(const QString& command)
{
    if (command == m_command) return;
    m_command = command;
    m_options.append(command);
	
    emit onOptionsChanged();	
    emit onCommandChanged();
}

QString BackEnd::currentProcess() const {
    return m_currentProcess;
}

QList<QString> BackEnd::options() const
{
    return m_options;
}

void BackEnd::setCurrentProcess(const QString& currentProcess) {
    if (currentProcess == m_currentProcess) return;
    m_currentProcess = currentProcess;
    emit onCurrentProcessChanged();
}

