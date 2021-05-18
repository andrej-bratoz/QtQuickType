#pragma once
#include <qlist.h>

#define QSTRING_PROP(name) \
	public: \
	QString Get##name() {return m_##name;} \
	void Set##name(QString value) {m_##name = value;} \
	private: \
	QString m_##name;
	

class Index
{
public:
	Index(QString path);
	QList<QString> _configurations;
};

class Command
{
public:
	Command(QString path);
	QSTRING_PROP(Cmd)
	QSTRING_PROP(Name)
	QSTRING_PROP(Parameter)
	QSTRING_PROP(Additional)
	QSTRING_PROP(Type)
	QSTRING_PROP(Winctx)
	QSTRING_PROP(Keys)
public:
	bool IsValid() const;

private:
	bool m_isValid;
};

class CommandList
{
public:
	explicit CommandList(const Index& index);
	void AddCommand(Command cmd);
private:
	QList<Command> _commands;
};
