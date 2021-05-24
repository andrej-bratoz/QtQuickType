#include "../inc/quicktype.h"




BackEnd::BackEnd(QObject* parent) : QObject(parent), m_command(""), m_proc(nullptr)
{
    const QString result(GetFullProcessName(GetForegroundWindow()));
	setCurrentProcess(result);
}

QString BackEnd::command() const{
    return m_command;
}

void BackEnd::setCommand(const QString& command)
{
    if (command == m_command) return;
    if (command.isEmpty()) clearCommandList();
    m_command = command;

    auto commands = m_index->FindCommand(command);
    m_options.clear();
    m_commands.clear();
	for (auto& cmd : commands)
	{
        if (cmd.GetShow()) {
            m_options.append(cmd.GetName());
            m_commands.append(cmd);
        }
	}

    if (m_commands.count() > 0) setSelectedIndex(0);

    emit onOptionsChanged();	
    emit onCommandChanged();
    emit onCommandsChanged();
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

void BackEnd::RegisterCmdIndex(CommandList* index)
{
    m_index = index;
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
    setCurrentProcess(GetFullProcessName(hwnd));
	m_activeWindow = GetTopWindow(hwnd);
}
