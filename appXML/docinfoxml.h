#ifndef DOCINFOXML_H
#define DOCINFOXML_H

#include "ydocsyetem.h"

#include <vector>

#include <QString>

/*!
 \brief 文档信息XML操作类

*/
class DocInfoXML
{
public:
    DocInfoXML();

    /*!
     \brief 创建文档信息XML

     \param docInfo 文档信息
     \param path 路径
     \return 成功返回true，否则返回false
    */
    static bool createDocInfo(const DocInfo *docInfo,const QString &path);

    /*!
     \brief 读取文档信息

     \param path 路径
     \return 文档信息
    */
    static DocInfo readDocInfo(const QString &path);

    /*!
     \brief 创建文件信息XML

     \param filesInfo 文件信息集合
     \param path
     \return 成功返回true，否则返回false
    */
    static bool createFilesInfo(const std::vector<FileInfo> &filesInfo,const QString &path);

    /*!
     \brief 从XML中读取文件信息

     \param path 路径
     \return 文件信息，个数为0时说明目录中没有文件
    */
    static std::vector<FileInfo> readFilesInfo(const QString &path);
};

#endif // DOCINFOXML_H
