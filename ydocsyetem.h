#ifndef YDOCSYETEM_H
#define YDOCSYETEM_H

#include <QString>
#include <QDateTime>
#include <QMetaType>

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
#define DEFINE_SYSTEM_INFO_TEXT "<systemInfo>\n    <version value=\"0.1.1\" />\n    <progectFile value=\"document.ydc\" />\n    <documentPath value=\"document\" />\n</systemInfo>"

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

//文档信息根节点XML标签
#define DOC_ROOT_XML_TAG "rootXML"

//默认的文档信息跟节点XML名称
#define DEFINE_ROOT_DOC_XML_NAME "docs"

//文档信息XML扩展名
#define DEFINE_DOC_INFO_XML_EXPANDED_NAME ".info"

//文档创建默认时间，当等于该时间时说明时间有问题
#define DOC_DEFINE_TIME "1900-01-01 0:00:00"

//日期时间格式
#define DATE_TIME_FORMAT "yyyy-MM-dd H:mm:ss"

/*!
 \brief 文件信息结构体

*/
typedef struct
{
    /*!
     \brief 文档信息中的文件类型

    */
    enum FileType
    {
        doc, /*!< 文档 */
        dir /*!< 目录 */
    };

    FileType fileType; /*!< 文件类型 */
    QString name; /*!< 文件名 */
    QString path; /*!< 所在路径 */
    QString title; /*!< 文件标题 */
    QDateTime createTime; /*!< 创建时间 */
    unsigned int childCount; /*!< 子节点个数 */
}FileInfo;
Q_DECLARE_METATYPE(FileInfo);
Q_DECLARE_METATYPE(FileInfo *);

//文件信息根节点
#define FILES_INFO_ROOT_TAG "filesInfo"

//文件信息类型是文档的节点名称
#define FILES_TYPE_FILE_TAG "doc"

//文件信息类型是目录的节点名称
#define FILES_TYPE_DIR_TAG "dir"

//文件获文件夹存储的文件名称节点
#define FILES_NAME_TAG "name"

//文件或文件夹所在路径
#define FILES_PATH_TAG "path"

//文档标题
#define FILES_TITLE_TAG "title"

//创建时间节点
#define FILES_CREATE_TIME_TAG "createTime"

//存储图片的文件夹名称
#define IMAGES_FILE_DIR "images"

//存储图片文件信息的文件扩展名，此系统使用文档名加扩展名来记录图片信息
#define IMAGES_INFO_FILE_EXPANDED_NAME ".dat"

//图像信息文件根节点标签
#define IMAGE_FILES_INFO_ROOT_TAG "images"

//图像信息文件节点标签
#define IMAGE_FILE_INFO_TAG "image"

//图像信息文件名标签
#define IMAGE_FILE_NAME_TAG "name"

//可以使用的图片类型
#define IMAGE_FILES_TYPE "所有图片 (*.jpg *.jpeg *.png *.bmp *.gif);;JPEG图片 (*.jpg *.jpeg);;png图片 (*.png);;位图 (*.bmp);;GIF图片 (*.gif)"

#endif // YDOCSYETEM_H
