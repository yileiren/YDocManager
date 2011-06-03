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
    }

    return docInfo;
}

bool DocInfoXML::createFilesInfo(const std::vector<FileInfo> &filesInfo, const QString &path)
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
    xmlWriter->writeStartElement(FILES_INFO_ROOT_TAG);

    //循环写入文件信息
    for(std::vector<FileInfo>::size_type s = 0;s < filesInfo.size();s++)
    {
        if(FileInfo::doc == filesInfo[s].fileType)
        {
            xmlWriter->writeStartElement(FILES_TYPE_FILE_TAG);
        }
        else
        {
            xmlWriter->writeStartElement(FILES_TYPE_DIR_TAG);
        }

        xmlWriter->writeAttribute(QObject::tr(FILES_NAME_TAG),filesInfo[s].name);
        xmlWriter->writeAttribute(QObject::tr(FILES_PATH_TAG),filesInfo[s].path);
        xmlWriter->writeAttribute(QObject::tr(FILES_TITLE_TAG),filesInfo[s].title);
        xmlWriter->writeAttribute(QObject::tr(FILES_CREATE_TIME_TAG),filesInfo[s].createTime.toString(QObject::tr(DATE_TIME_FORMAT)));
        xmlWriter->writeEndElement();

    }

    xmlWriter->writeEndElement();

    xmlWriter->writeEndDocument();
    delete xmlWriter;
    file.close();
    return true;
}

std::vector<FileInfo> DocInfoXML::readFilesInfo(const QString &path)
{
    //检查文档信息
    QFile docInfoFile;
    docInfoFile.setFileName(path);

    std::vector<FileInfo> filesInfo;

    //打开文件
    if(docInfoFile.open(QFile::ReadOnly))
    {
        //读取文档信息
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&docInfoFile);
        while(!xmlReader.atEnd())
        {
            FileInfo fileInfo;

            if(xmlReader.isStartElement())
            {
                QString name = xmlReader.name().toString();
                if(name == FILES_TYPE_FILE_TAG)
                {
                    fileInfo.fileType = FileInfo::doc;
                    fileInfo.name = xmlReader.attributes().value(QObject::tr(FILES_NAME_TAG)).toString();
                    fileInfo.path = xmlReader.attributes().value(QObject::tr(FILES_PATH_TAG)).toString();
                    fileInfo.title = xmlReader.attributes().value(QObject::tr(FILES_TITLE_TAG)).toString();
                    fileInfo.createTime = QDateTime::fromString(xmlReader.attributes().value(QObject::tr(FILES_CREATE_TIME_TAG)).toString(),DATE_TIME_FORMAT);
                    filesInfo.push_back(fileInfo);
                }
                else if(name == FILES_TYPE_DIR_TAG)
                {
                    fileInfo.fileType = FileInfo::dir;
                    fileInfo.name = xmlReader.attributes().value(QObject::tr(FILES_NAME_TAG)).toString();
                    fileInfo.path = xmlReader.attributes().value(QObject::tr(FILES_PATH_TAG)).toString();
                    fileInfo.title = xmlReader.attributes().value(QObject::tr(FILES_TITLE_TAG)).toString();
                    fileInfo.createTime = QDateTime::fromString(xmlReader.attributes().value(QObject::tr(FILES_CREATE_TIME_TAG)).toString(),DATE_TIME_FORMAT);
                    filesInfo.push_back(fileInfo);
                }



                xmlReader.readNext();
            }
            else
            {
                xmlReader.readNext();
            }
        }
        docInfoFile.close();
    }

    return filesInfo;
}
