#ifndef COMMONTOOL_FILEOPERATOR_H
#define COMMONTOOL_FILEOPERATOR_H

#include "../../common_tool_global.h"

#include <QFileInfoList>
#include <QFile>
#include <QDir>
#include <QList>
#include <QStringList>

class COMMON_TOOL_EXPORT CommonTool_FileOperator : public QObject
{
	Q_OBJECT
public:

	static int isDirOrFile(QString fullStr);
	static bool isDirExist(QString fullPath);
	static bool isFileExist(QString fullFileName);
	static bool createDir(QString fullPath);
	static bool createPath(QString fullPath);
	static bool createFile(QString fullFileName);
	static bool removeFile(QString fullFileName);
	static bool removeFiles(std::vector<QString> fullFileNames);
	static bool removeFiles(QStringList lststrFullFileNames);
	static bool removeDirRec(QString fullPath);
	static bool removeFlatDirFiles(QString fullPath);
	static bool copyFile(QString fullFileName, QString newFullFileName);
	static bool copyFiles(std::vector<QString> fullFileNames, QString newDir);
	static bool moveFile(QString fullFileName, QString newFullFileName);
	static bool moveFiles(std::vector<QString> fullFileNames, QString newDir);
	static std::vector<QString> getFileList(const QString& dir, bool isFullPath = true);
	static std::vector<QString> getDirList(const QString& dir, bool isFullPath = true);

	/// <summary>
	/// 获得文件夹下的文件夹路径
	/// </summary>
	/// <param name="strDir">文件夹路径</param>
	/// <param name="plistDirPath">存放文件夹下的所有文件夹路径，返回值</param>
	/// <param name="bIsFullPath">true代表是完整的路径，false仅代表文件夹的名字</param>
	static void getDirList(const QString strDir, QList<QString>* plistDirPath, bool bIsFullPath = true);
	static std::vector<QString> getDirFileList(const QString& dir, bool isFullPath = true);
	static std::vector<QString> getSpecialFileList(const QString& dir, const QString& specialName, int specialNameType, const std::vector<QString>& specialSuffix, bool isFullPath = true);
	static QFileInfoList getFileListByTraverseFolder(QString path);

	static void removeEmptyDirectoryByTraverseFolder(QString path);

	// sort file list by name compare with char, big->small
	static bool sortFileList(std::vector<QString>& fileList);

	// read all data from file
	static bool readAllDataFromFile(QString file_name, std::vector<QString>& all_line_data);
	// write all data into file
	static bool writeAllDataIntoFile(QString file_name, std::vector<QString>& all_line_data, bool is_supersede, bool is_add_line_feed);

	// get current time stamp
	static QString getCurrentTimeStamp(); //20001010-12:12:12.121
	static QString getCurrentTimeStamp1(); //12:12:12.121
	static QString getCurrentTimeStamp2(); //20001010-121212121
	static QString getCurrentTimeStamp3(); //20001010121212121
	static QString getCurrentTimeStamp4(); //121212121

	// get current date stamp
	static QString getCurrentDateStamp();
	static QString getCurrentDateStamp1();

	/// <summary>
	/// 分割文件的绝对路径
	/// </summary>
	/// <param name="strFilePathname">文件的绝对路径</param>
	/// <returns>返回值，包含文件的路径和文件名的字符串列表</returns>
	static QStringList splitFilePathname(QString strFilePathname);

	/// <summary>
	/// 分割文件的名字
	/// </summary>
	/// <param name="strFileName">文件的名字</param>
	/// <returns>返回值，包含文件名称和文件后缀</returns>
	static QStringList splitFileName(QString strFileName);
};

#endif // COMMONTOOL_FILEOPERATOR_H