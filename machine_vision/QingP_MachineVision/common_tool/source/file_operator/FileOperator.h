#ifndef COMMONTOOL_FILEOPERATOR_H
#define COMMONTOOL_FILEOPERATOR_H

#include "../../common_tool_global.h"

#include <QFileInfoList>
#include <QFile>
#include <QDir>
#include <QList>
#include <QStringList>

namespace CommonTool_FileOperator
{
	int isDirOrFile(QString fullStr);
	bool isDirExist(QString fullPath);
	bool isFileExist(QString fullFileName);
	bool createDir(QString fullPath);
	bool createPath(QString fullPath);
	bool createFile(QString fullFileName);
	bool removeFile(QString fullFileName);
	bool removeFiles(std::vector<QString> fullFileNames);
	bool removeFiles(QStringList lststrFullFileNames);
	bool removeDirRec(QString fullPath);
	bool removeFlatDirFiles(QString fullPath);
	bool copyFile(QString fullFileName, QString newFullFileName);
	bool copyFiles(std::vector<QString> fullFileNames, QString newDir);
	bool moveFile(QString fullFileName, QString newFullFileName);
	bool moveFiles(std::vector<QString> fullFileNames, QString newDir);
	std::vector<QString> getFileList(const QString& dir, bool isFullPath = true);
	std::vector<QString> getDirList(const QString& dir, bool isFullPath = true);

	/// <summary>
	/// 获得文件夹下的文件夹路径
	/// </summary>
	/// <param name="strDir">文件夹路径</param>
	/// <param name="plistDirPath">存放文件夹下的所有文件夹路径，返回值</param>
	/// <param name="bIsFullPath">true代表是完整的路径，false仅代表文件夹的名字</param>
	void getDirList(const QString strDir, QList<QString>* plistDirPath, bool bIsFullPath = true);
	std::vector<QString> getDirFileList(const QString& dir, bool isFullPath = true);
	std::vector<QString> getSpecialFileList(const QString& dir, const QString& specialName, int specialNameType, const std::vector<QString>& specialSuffix, bool isFullPath = true);
	QFileInfoList getFileListByTraverseFolder(QString path);

	void removeEmptyDirectoryByTraverseFolder(QString path);

	// sort file list by name compare with char, big->small
	bool sortFileList(std::vector<QString>& fileList);

	// read all data from file
	bool readAllDataFromFile(QString file_name, std::vector<QString>& all_line_data);
	// write all data into file
	bool writeAllDataIntoFile(QString file_name, std::vector<QString>& all_line_data, bool is_supersede, bool is_add_line_feed);

	// get current time stamp
	QString getCurrentTimeStamp(); //20001010-12:12:12.121
	QString getCurrentTimeStamp1(); //12:12:12.121
	QString getCurrentTimeStamp2(); //20001010-121212121
	QString getCurrentTimeStamp3(); //20001010121212121
	QString getCurrentTimeStamp4(); //121212121

	// get current date stamp
	QString getCurrentDateStamp();
	QString getCurrentDateStamp1();
};

#endif // COMMONTOOL_FILEOPERATOR_H