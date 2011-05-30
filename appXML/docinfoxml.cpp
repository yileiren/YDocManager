#include "docinfoxml.h"

#include <QXmlStreamWriter>
#include <QFile>

DocInfoXML::DocInfoXML()
{
}

bool DocInfoXML::createDocInfo(const DocInfo *docInfo,const QString &path)
{
    QFile file(path);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        return false;
    }

    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(&file);
    xmlWriter->setAutoFormatting(true);
    xmlWriter->writeStartDocument();

    //写入跟节点
    xmlWriter->writeStartElement(DOC_INFO_TAG);

    //uuid节点
    xmlWriter->writeStartElement(DOC_UUID_TAG);
    xmlWriter->writeAttribute(QObject::tr("value"),docInfo->uuid);
    xmlWriter->writeEndElement();

    //名字节点
    xmlWriter->writeStartElement(DOC_NAME_TAG);
    xmlWriter->writeAttribute(QObject::tr("value"),docInfo->name);
    xmlWriter->writeEndElement();

    //编号节点
    xmlWriter->writeStartElement(DOC_NUM_TAG);
    xmlWriter->writeAttribute(QObject::tr("value"),docInfo->num);
    xmlWriter->writeEndElement();

    //用户名节点
    xmlWriter->writeStartElement(DOC_USER_NAME_TAG);
    xmlWriter->writeAttribute(QObject::tr("value"),docInfo->userName);
    xmlWriter->writeEndElement();

    //用户编号节点
    xmlWriter->writeStartElement(DOC_USER_NUM_TAG);
    xmlWriter->writeAttribute(QObject::tr("value"),docInfo->userNum);
    xmlWriter->writeEndElement();

    //创建时间节点
    xmlWriter->writeStartElement(DOC_CREATE_TIME_TAG);
    xmlWriter->writeAttribute(QObject::tr("value"),docInfo->createTime.toString(QObject::tr(DATE_TIME_FORMAT)));
    xmlWriter->writeEndElement();

    //跟文档跟节点
    xmlWriter->writeStartElement(DOC_ROOT_XML_TAG);
    xmlWriter->writeAttribute(QObject::tr("value"),docInfo->rootXML);
    xmlWriter->writeEndElement();

    xmlWriter->writeEndElement();

    xmlWriter->writeEndDocument();
    delete xmlWriter;
    file.close();
    return true;
}

DocInfo DocInfoXML::readDocInfo(const QString &path)
{
    //检查文档信息
    QFile docInfoFile;
    docInfoFile.setFileName(path);

    //初始化文档信息
    DocInfo docInfo;
    docInfo.createTime = QDateTime::fromString(QObject::tr(DOC_DEFINE_TIME),QObject::tr(DATE_TIME_FORMAT));
    docInfo.name = QObject::tr("");
    docInfo.num = QObject::tr("");
    docInfo.rootXML = QObject::tr("");
    docInfo.userName = QObject::tr("");
    docInfo.userNum = QObject::tr("");
    docInfo.uuid = QObject::tr("");

    if(docInfoFile.open(QFile::ReadOnly))
    {
        //读取文档信息
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&docInfoFile);
        while(!xmlReader.atEnd())
        {
            if(xmlReader.isStartElement())
            {
                QString name = xmlReader.name().toString();
                if(name == DOC_UUID_TAG)
                {
                    docInfo.uuid = xmlReader.attributes().value(QObject::tr("value")).toString();
                }
                else if(name == DOC_NAME_TAG)
                {
                    docInfo.name = xmlReader.attributes().value(QObject::tr("value")).toString();
                }
                else if(name == DOC_NUM_TAG)
                {
                    docInfo.num = xmlReader.attributes().value(QObject::tr("value")).toString();
                }
                else if(name == DOC_USER_NAME_TAG)
                {
                    docInfo.userName = xmlReader.attributes().value(QObject::tr("value")).toString();
                }
                else if(name == DOC_USER_NUM_TAG)
                {
                    docInfo.userNum = xmlReader.attributes().value(QObject::tr("value")).toString();
                }
                else if(name == DOC_CREATE_TIME_TAG)
                {
                    docInfo.createTime = QDateTime::fromString(xmlReader.attributes().value(QObject::tr("value")).toString(),DATE_TIME_FORMAT);
                }
                else if(name == DOC_ROOT_XML_TAG)
                {
                    docInfo.rootXML = xmlReader.attributes().value(QObject::tr("value")).toString();
                }

                xmlReader.readNext();
            }
            else
            {
                xmlReader.readNext();
            }
        }
        docInfoFile.close();
        docInfoFile.close();
    }

    return docInfo;
}
