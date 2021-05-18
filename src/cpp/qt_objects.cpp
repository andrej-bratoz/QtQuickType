#include <QXmlStreamReader>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>
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


Command::Command(QString path) : m_Cmd(""),m_Name(""), m_Parameter(""),
                                 m_Additional(""), m_Type(""), m_Winctx(""), m_Keys("")
{

	QFile file(path);
	if (!file.exists(path)) 
	{
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
		if(xml.isStartElement())
		{
			if(xml.name().toString() == "command")
			{
				if(xml.attributes().hasAttribute("cmd"))
					SetCmd(xml.attributes().value("cmd").toString());
				if (xml.attributes().hasAttribute("name"))
					SetName(xml.attributes().value("name").toString());
				if (xml.attributes().hasAttribute("parameter"))
					SetParameter(xml.attributes().value("parameter").toString());
				if (xml.attributes().hasAttribute("additional"))
					SetAdditional(xml.attributes().value("additional").toString());
				if (xml.attributes().hasAttribute("type"))
					SetType(xml.attributes().value("type").toString());
				if (xml.attributes().hasAttribute("winctx"))
					SetWinctx(xml.attributes().value("winctx").toString());
				if (xml.attributes().hasAttribute("keys"))
					SetKeys(xml.attributes().value("keys").toString());
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

CommandList::CommandList(const Index& index)
{
	for(const auto& item : index._configurations)
	{
		Command cmd(item);
		if (cmd.IsValid()) {
			AddCommand(cmd);
		}
	}
}

void CommandList::AddCommand(Command cmd)
{
	this->_commands.push_back(cmd);
}
