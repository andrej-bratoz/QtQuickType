#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QQmlContext>
#include <qqml.h>
#include <Shlwapi.h>
#include <Windows.h>
#include <utility>
#include <Psapi.h>

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
    WINEVENTPROC m_proc{};
    QString m_command;
    QString m_currentProcess;
    QList<QString> m_options;
    int m_selectedIndex = -1;
};

inline HWND FindTopWindow(DWORD pid)
{
    std::pair<HWND, DWORD> params = { 0, pid };

    // Enumerate the windows using a lambda to process each window
    BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL
        {
            auto pParams = (std::pair<HWND, DWORD>*)(lParam);

            DWORD processId;
            if (GetWindowThreadProcessId(hwnd, &processId) && processId == pParams->second)
            {
                // Stop enumerating
                SetLastError(-1);
                pParams->first = hwnd;
                return FALSE;
            }

            // Continue enumerating
            return TRUE;
        }, (LPARAM)&params);

    if (!bResult && GetLastError() == -1 && params.first)
    {
        return params.first;
    }

    return 0;
}

inline QString GetFullProcessName(HWND handle)
{
    DWORD processId;
    DWORD charsWritten = 0;
    wchar_t windowText[70];
    wchar_t procName[MAX_PATH];

    ZeroMemory(windowText, 70 * sizeof(wchar_t));
    ZeroMemory(procName, MAX_PATH * sizeof(wchar_t));

    ////
    GetWindowTextW(handle, windowText, 70);
    auto text = std::wstring(windowText);
    text += L"...";

	GetWindowThreadProcessId(handle, &processId);
	HANDLE phandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
    GetProcessImageFileName(phandle, procName, MAX_PATH);
    CloseHandle(phandle);
    	
    const LPWSTR fileName = PathFindFileNameW(procName);

    QString result(QString::fromWCharArray(text.c_str()));
	result += " | ";
    result += fileName;

    return result;
}


#endif // BACKEND_H
