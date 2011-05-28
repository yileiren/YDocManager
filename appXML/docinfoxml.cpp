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
