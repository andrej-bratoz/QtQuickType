#include "quicktype.h"

BackEnd::BackEnd(QObject* parent) : QObject(parent), m_command(""),
	m_selectedIndex(-1)
{
}

QString BackEnd::command() const{
    return m_command;
}

void BackEnd::setCommand(const QString& command)
{
    if (command == m_command) return;
    if (command.isEmpty()) clearCommandList();
    m_command = command;
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

int BackEnd::selectedIndex() const
{
    return m_selectedIndex;
}

void BackEnd::setCurrentProcess(const QString& currentProcess) {
    if (currentProcess == m_currentProcess) return;
    m_currentProcess = currentProcess;
    emit onCurrentProcessChanged();
}

void BackEnd::setSelectedIndex(int index)
{
    if (m_selectedIndex == index) return;
    m_selectedIndex = index;
    setCurrentProcess("Current Selection: " + QString::number(index));
    emit onOptionSelected(index);
}

void BackEnd::clearCommandList()
{
    m_options.clear();
    m_options.detach();
    setSelectedIndex(-1);
}

void BackEnd::handleWindowsEventHookCallback(HWINEVENTHOOK hWinEventHook, uint eventType, HWND hwnd, LONG idObject,
	LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
    char title[1024];
    GetWindowTextA(hwnd, title, 1024);
    setCurrentProcess(title);
}
