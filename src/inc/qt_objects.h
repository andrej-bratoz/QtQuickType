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


class Index
{
public:
	Index(QString path);
	QList<QString> _configurations;
	bool IsValid() const;

private:
	bool m_isValid;
};

class Command
{
public:
	Command(QString path);
	void ReadCommandFromFile();

	QSTRING_PROP(Name)
	QSTRING_PROP(ProcessFilter)
	QSTRING_PROP(Cmd)
	QSTRING_PROP(Parameter)
	QSTRING_PROP(Type)
	QBOOL_PROP(Show)
	QSTRING_PROP(SendKeys)

public:
	bool IsValid() const;
	void AddCommand();
	CommandTypeEnum ActualType() const { return m_typeEnum; }
	void SetActualType(CommandTypeEnum value) { m_typeEnum = value; }
	[[nodiscard]] std::vector<Command> GetCommands() const { return m_commands; }
	void Execute(HWND activeWindow) const;

private:
	bool m_isValid;
	QString m_path;
	std::vector<Command> m_commands;
	CommandTypeEnum m_typeEnum;
};

class CommandList
{
public:
	explicit CommandList(const Index& index);
	void AddCommand(Command cmd);
	[[nodiscard]] std::vector<Command> FindCommand(const QString& name) const;
private:
	QList<Command> _commands;
};
