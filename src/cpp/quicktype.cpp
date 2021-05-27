#include "../inc/quicktype.h"




QuickTypeBL::QuickTypeBL(QObject* parent) : QObject(parent),
								m_index(nullptr),
								m_proc(nullptr),
								m_command(""),
								m_activeWindow(nullptr)
{
    const QString result(GetFullProcessName(GetForegroundWindow()));
	setCurrentProcess(result);
}

QList<Command> QuickTypeBL::commands() const
{
	return m_commands;
}

QString QuickTypeBL::command() const{
    return m_command;
}

void QuickTypeBL::setCommand(const QString& command)
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

    auto commands = m_index->findCommand(command);
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

QString QuickTypeBL::currentProcess() const
{
    return m_currentProcess;
}

QList<QString> QuickTypeBL::options() const
{
    return m_options;
}

int QuickTypeBL::selectedIndex() const
{
    return m_selectedIndex;
}

int QuickTypeBL::count() const
{
	return m_commands.count();
}

void QuickTypeBL::registerCmdIndex(CommandList* index)
{
    m_index = index;
}

QString QuickTypeBL::activeCommand()
{
	return (m_commands[selectedIndex()].GetName());
}

void QuickTypeBL::execCurrentCmd()
{
	if (selectedIndex() < 0) return;
	if (selectedIndex() >= m_commands.count()) return;
	m_commands[selectedIndex()].execute(m_activeWindow);
}

void QuickTypeBL::setCurrentProcess(const QString& currentProcess) {
    if (currentProcess == m_currentProcess) return;
    m_currentProcess = currentProcess;
    emit onCurrentProcessChanged();
}

void QuickTypeBL::setSelectedIndex(int index)
{
    if (m_selectedIndex == index) return;
    if (index >= count()) return;
    if (index < 0) return;
    m_selectedIndex = index;
    emit onOptionSelected(index);
}

void QuickTypeBL::clearCommandList()
{
    m_options.clear();
    m_options.detach();
    setSelectedIndex(-1);
}

void QuickTypeBL::handleWindowsEventHookCallback(HWINEVENTHOOK hWinEventHook, uint eventType, HWND hwnd, LONG idObject,
	LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{   
    setCurrentProcess(GetFullProcessName(hwnd));
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);
	
 	m_activeWindow = FindTopWindow(processId);
}
