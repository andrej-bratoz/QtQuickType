#pragma once
#include <qlist.h>

#define QSTRING_PROP(name) \
	public: \
	QString Get##name() const {return m_##name;} \
	void Set##name(QString value) {m_##name = value;} \
	private: \
	QString m_##name;

#define QBOOL_PROP(name) \
	public: \
	bool Get##name() const {return m_##name;} \
	void Set##name(bool value) {m_##name = value;} \
	private: \
	bool m_##name;



enum class CommandTypeEnum
{
	Unknown,
	WinCreate,
	Keys,
	SpawnProcess
};

class Command
{
public:
	Command(QString path);
	void readCommandFromFile();
	QSTRING_PROP(Name)
	QSTRING_PROP(ProcessFilter)
	QSTRING_PROP(Cmd)
	QSTRING_PROP(Parameter)
	QSTRING_PROP(Type)
	QBOOL_PROP(Show)
	QSTRING_PROP(SendKeys)

public:
	[[nodiscard]] bool isValid() const;
	void addCommand();
	[[nodiscard]] CommandTypeEnum getActualType() const;
	void setActualType(CommandTypeEnum value);
	[[nodiscard]] std::vector<Command> GetCommands() const;
	void execute(HWND activeWindow) const;

private:
	bool m_isValid;
	QString m_path;
	std::vector<Command> m_commands;
	CommandTypeEnum m_typeEnum;
};

class Index
{
public:
	Index(const QString& path);
	QList<QString> _configurations;
	[[nodiscard]] bool isValid() const;

private:
	bool m_isValid;
};

class CommandList
{
public:
	explicit CommandList(const Index& index);
	void addCommand(Command cmd);
	[[nodiscard]] std::vector<Command> findCommand(const QString& name) const;
private:
	QList<Command> _commands;
};

