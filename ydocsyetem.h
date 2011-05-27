#ifndef YDOCSYETEM_H
#define YDOCSYETEM_H

#include <QString>
#include <QDateTime>

//系统信息文件路径
#define SYSTEM_INFO_FILE "systemInfo.xml"

//当前系统版本号标签名称
#define SYSTEM_VERSION_TAG "version"

//系统存放文档路径标签名称
#define SYSTEM_DOCUMENT_TAG "documentPath"

//HTML格式文档存储的文件夹名
#define HTML_DOC_PATH "HTML"

//TEXT格式文档存储的文件夹名
#define TEXT_DOC_PATH "TEXT"

//工程文件标签
#define SYSTEM_PROGECT_FILE_TAG "progectFile"

//默认系统信息
#define DEFINE_SYSTEM_INFO_TEXT "<systemInfo>\n    <version value=\"0.1.0\" />\n    <progectFile value=\"document.ydc\" />\n    <documentPath value=\"document\" />\n</systemInfo>"

//工程文件默认扩展名
#define EXPANDED_NAME ".ydc"

/*!
 \brief 系统信息结构体

*/
typedef struct
{
    QString version; /*!< 系统版本号 */
    QString documentPath; /*!< 文档路径 */
    QString progectFile; /*!< 工程文件 */
}SystemInfo;

/*!
 \brief 文档信息结构体

*/
typedef struct
{
    QString uuid; /*!< 文档唯一标识 */
    QString name; /*!< 文档名称 */
    QString num; /*!< 文档编号 */
    QString userName; /*!< 用户名 */
    QString userNum; /*!< 用户编号 */
    QDateTime createTime; /*!< 文档创建时间 */
    QString rootXML; /*!< 文档根节点XML路径 */
}DocInfo;

//文档信息标签
#define DOC_INFO_TAG "docInfo"

//文档唯一标识标签
#define DOC_UUID_TAG "uuid"

//文档名称标签
#define DOC_NAME_TAG "name"

//文档编号标签
#define DOC_NUM_TAG "num"

//用户名标签
#define DOC_USER_NAME_TAG "userName"

//用户编号标签
#define DOC_USER_NUM_TAG "userNum"

//文档创建时间
#define DOC_CREATE_TIME_TAG "createTime"

//文档根节点XML路径
#define DOC_ROOT_XML_TAG "rootXML"

//默认的跟节点XML名称
#define DEFINE_ROOT_DOC_XML_NAME "root"

//文档创建默认时间，当等于该时间时说明时间有问题
#define DOC_DEFINE_TIME "1900-01-01 0:00:00"

//日期时间格式
#define DATE_TIME_FORMAT "yyyy-MM-dd H:mm:ss"
#endif // YDOCSYETEM_H
