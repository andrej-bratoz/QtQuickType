#include "../inc/quicktype.h"




BackEnd::BackEnd(QObject* parent) : QObject(parent),
								m_index(nullptr),
								m_proc(nullptr),
								m_command(""),
								m_activeWindow(nullptr)
{
    const QString result(GetFullProcessName(GetForegroundWindow()));
	setCurrentProcess(result);
}

QString BackEnd::command() const{
    return m_command;
}

void BackEnd::setCommand(const QString& command)
{
    if(command.isEmpty())
    {
        m_commands.clear();
        m_options.clear();
        m_command = "";
        m_selectedIndex = -1;
        clearCommandList();
        emit onOptionsChanged();
    	return;
    }
	
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

    if (m_commands.count() > 0)
        setSelectedIndex(0);
    else if (m_commands.count() == 0)
        setSelectedIndex(-1);

    emit onOptionsChanged();	
    emit onCommandChanged();
    emit onCommandsChanged();
    emit onCountChanged();
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
    if (index >= count()) return;
    if (index < 0) return;
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
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);
	
 	m_activeWindow = FindTopWindow(processId);
}
