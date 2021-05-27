#include <QXmlStreamReader>
#include <QFile>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <Windows.h>
#include "../ext/SendKeys.h"
#include "../inc/xml_objects.h"

// INDEX

Index::Index(const QString& path)
{
	QFile file(path);
	if (!QFile::exists(path))
	{
		m_isValid = false;
		return;
	}
	m_isValid = true;
	file.open(QIODeviceBase::ReadOnly);
	QXmlStreamReader xml;
	xml.setDevice(&file);
	xml.readNext();
	while (!xml.isEndDocument())
	{
		if (xml.isStartElement())
		{
			if (xml.name().toString() == "cmdfile")
			{
				auto text = xml.attributes().value("path").toString();
				const QDir info(text);
				const QString fullPath(info.absolutePath());
				_configurations.append(fullPath);
			}
		}
		xml.readNext();
	}
	file.close();
}

bool Index::isValid() const
{
	return m_isValid;
}

// COMMAND
Command::Command(QString path) : m_Name(""), m_ProcessFilter(""), m_Cmd(""),
                                 m_Parameter(""), m_Type(""), m_Show(true),
                                 m_SendKeys(""), m_isValid(false), m_path(std::move(path)),
                                 m_typeEnum(CommandTypeEnum::Unknown)
{
}

void Command::readCommandFromFile()
{
	QFile file(m_path);
	if (!QFile::exists(m_path))
	{
		m_isValid = false;
		return;
	}
	m_isValid = true;
	file.open(QIODeviceBase::ReadOnly);
	QXmlStreamReader xml;
	xml.setDevice(&file);
	xml.readNext();
	while (!xml.isEndDocument())
	{
		if (xml.isStartElement())
		{
			if (xml.name().toString() == "command")
			{
				if (xml.attributes().hasAttribute("name"))
					SetName(xml.attributes().value("name").toString());

				if (xml.attributes().hasAttribute("cmd"))
					SetCmd(xml.attributes().value("cmd").toString());

				if (xml.attributes().hasAttribute("parameter"))
					SetParameter(xml.attributes().value("parameter").toString());

				if (xml.attributes().hasAttribute("type"))
					SetType(xml.attributes().value("type").toString());

				if (xml.attributes().hasAttribute("show"))
					SetShow(xml.attributes().value("show").toInt() == 1);

				if (xml.attributes().hasAttribute("sendkeys"))
					SetSendKeys(xml.attributes().value("sendkeys").toString());

				if (xml.attributes().hasAttribute("procfilter"))
					SetProcessFilter(xml.attributes().value("procfiler").toString());

				if (GetType() == "wincreate") setActualType(CommandTypeEnum::WinCreate);
				else if (GetType() == "keys") setActualType(CommandTypeEnum::Keys);
				else if (GetType() == "spawnproc") setActualType(CommandTypeEnum::SpawnProcess);

				if (GetShow()) addCommand();
			}
		}
		xml.readNext();
	}

	file.close();
}

bool Command::isValid() const
{
	return m_isValid;
}

void Command::addCommand()
{
	if (this->getActualType() == CommandTypeEnum::Unknown) return;
	Command cmd(m_path);
	cmd.SetSendKeys(this->GetSendKeys());
	cmd.SetCmd(this->GetCmd());
	cmd.SetName(this->GetName());
	cmd.SetParameter(this->GetParameter());
	cmd.SetShow(this->GetShow());
	cmd.SetType(this->GetType());
	cmd.SetProcessFilter(this->GetProcessFilter());
	cmd.setActualType(this->getActualType());
	this->m_commands.push_back(cmd);
}

CommandTypeEnum Command::getActualType() const
{
	return m_typeEnum;
}

void Command::setActualType(CommandTypeEnum value)
{
	m_typeEnum = value;
}

std::vector<Command> Command::GetCommands() const
{
	return m_commands;
}

void Command::execute(HWND activeWindow) const
{
	if (getActualType() == CommandTypeEnum::Keys)
	{
		CSendKeys sk;
		const QString name3 = GetName();
		const QString keys = GetSendKeys();
		const std::wstring name(keys.toStdWString());
		const LPCTSTR string = name.c_str();
		wchar_t title[1024];
		GetWindowTextW(activeWindow, title, 1024);
		qDebug() << QString::fromWCharArray(L"Active Window: ") << QString::fromWCharArray(title);
		CSendKeys::AppActivate(activeWindow);
		sk.SendKeys(string, true);
	}
}


// COMMAND LIST

CommandList::CommandList(const Index& index)
{
	for (const auto& item : index._configurations)
	{
		Command cmd(item);
		cmd.readCommandFromFile();
		for (const auto& cmd1 : cmd.GetCommands())
		{
			addCommand(cmd1);
		}
	}
}

void CommandList::addCommand(Command cmd)
{
	this->_commands.push_back(cmd);
}

std::vector<Command> CommandList::findCommand(const QString& name) const
{
	std::vector<Command> _result;
	for (const Command& cmd : _commands)
	{
		if (cmd.GetName().startsWith(name))
		{
			_result.push_back(cmd);
		}
	}
	return _result;
}
