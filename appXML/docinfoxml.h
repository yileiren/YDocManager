#ifndef DOCINFOXML_H
#define DOCINFOXML_H

#include "ydocsyetem.h"

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
};

#endif // DOCINFOXML_H
