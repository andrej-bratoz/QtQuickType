#include <QXmlStreamReader>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <Windows.h>
#include "../ext/SendKeys.h"
#include "../inc/qt_objects.h"



Index::Index(QString path)
{
	QFile file(path);
	if (!file.exists(path)) {
		m_isValid = false;
		return;
	}
	m_isValid = true;
	file.open(QIODeviceBase::ReadOnly);
	QXmlStreamReader xml;
	xml.setDevice(&file);
	xml.readNext();
	while(!xml.isEndDocument())
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

bool Index::IsValid() const
{
	return m_isValid;
}


Command::Command(QString path) : m_Name(""),m_ProcessFilter(""), m_Cmd(""),
									m_Parameter(""),m_Type(""),m_Show(true),
									m_SendKeys(""), m_isValid(false), m_path(std::move(path)),
									m_typeEnum(CommandTypeEnum::Unknown)
{

	
}

void Command::ReadCommandFromFile()
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

				if (GetType() == "wincreate") SetActualType(CommandTypeEnum::WinCreate);
				else if (GetType() == "keys") SetActualType(CommandTypeEnum::Keys);
				else if (GetType() == "spawnproc") SetActualType(CommandTypeEnum::SpawnProcess);
								
				if(GetShow()) AddCommand();
			}
		}
		xml.readNext();
	}

	file.close();
}

bool Command::IsValid() const
{
	return m_isValid;
}

void Command::AddCommand()
{
	if (this->ActualType() == CommandTypeEnum::Unknown) return;	
	Command cmd(m_path);
	cmd.SetSendKeys(this->GetSendKeys());
	cmd.SetCmd(this->GetCmd());
	cmd.SetName(this->GetName());
	cmd.SetParameter(this->GetParameter());
	cmd.SetShow(this->GetShow());
	cmd.SetType(this->GetType());
	cmd.SetProcessFilter(this->GetProcessFilter());
	cmd.SetActualType(this->ActualType());
	this->m_commands.push_back(cmd);
}

void Command::Execute(HWND activeWindow) const
{
	if(ActualType() == CommandTypeEnum::Keys)
	{
		CSendKeys sk;
		const QString name3 = GetName();
		const QString keys = GetSendKeys();
		const std::wstring name(keys.toStdWString());
		const LPCTSTR string = name.c_str();
		CSendKeys::AppActivate(activeWindow);
		sk.SendKeys(string, true);
	}
}


CommandList::CommandList(const Index& index)
{
	for(const auto& item : index._configurations)
	{
		Command cmd(item);
		cmd.ReadCommandFromFile();
		for (const auto& cmd1: cmd.GetCommands())
		{
			AddCommand(cmd1);
		}
	}
}

void CommandList::AddCommand(Command cmd)
{
	this->_commands.push_back(cmd);
}

std::vector<Command> CommandList::FindCommand(const QString& name) const
{
	std::vector<Command> _result;
	for(const Command& cmd : _commands)
	{
		if(cmd.GetName().startsWith(name))
		{
			_result.push_back(cmd);
		}
	}
	return _result;
}

