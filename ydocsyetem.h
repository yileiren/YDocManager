#ifndef YDOCSYETEM_H
#define YDOCSYETEM_H

#include <QString>

//系统信息文件路径
#define SYSTEM_INFO_FILE "systemInfo.xml"

//当前系统版本号标签名称
#define SYSTEM_VERSION_TAG "version"

//系统存放文档路径标签名称
#define SYSTEM_DOCUMENT_TAG "documentPath"

//工程文件标签
#define SYSTEM_PROGECT_FILE_TAG "progectFile"

//默认系统信息
#define DEFINE_SYSTEM_INFO_TEXT "<systemInfo>\n    <version value=\"0.1.0\" />\n    <progectFile value=\"document.ydc\" />\n    <documentPath value=\"document\" />\n</systemInfo>"

/*!
 \brief 系统信息结构体

*/
typedef struct
{
    QString version; /*!< 系统版本号 */
    QString documentPath; /*!< 文档路径 */
    QString progectFile; /*!< 工程文件 */
}SystemInfo;

#endif // YDOCSYETEM_H
