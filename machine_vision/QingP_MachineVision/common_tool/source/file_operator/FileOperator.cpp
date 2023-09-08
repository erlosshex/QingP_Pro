#include "FileOperator.h"

using namespace std;

int CommonTool_FileOperator::isDirOrFile(QString fullStr)
{
	QFileInfo fi(fullStr);

	if (fi.isDir() || fi.isFile()) {
		if (fi.isDir()) {
			return 1;
		}
		else {
			return 2;
		}
	}
	else {
		return 0;
	}
}

bool CommonTool_FileOperator::isDirExist(QString fullPath)
{
	QDir dir(fullPath);
	if (dir.exists()) {
		return true;
	}
	else {
		return false;
	}
}

bool CommonTool_FileOperator::isFileExist(QString fullFileName)
{
	QFile file(fullFileName);
	if (file.exists()) {
		return true;
	}
	else {
		return false;
	}
}

bool CommonTool_FileOperator::createDir(QString fullPath)
{
	QDir dir(fullPath);
	if (dir.exists()) {
		return true;
	}
	else {
		return dir.mkdir(fullPath);
	}
}

bool CommonTool_FileOperator::createPath(QString fullPath)
{
	QDir dir(fullPath);
	if (dir.exists()) {
		return true;
	}
	else {
		return dir.mkpath(fullPath);
	}
}

bool CommonTool_FileOperator::createFile(QString fullFileName)
{
	QFile file(fullFileName);
	if (file.exists()) {
		return true;
	}
	else {
		file.open(QIODevice::ReadWrite);
		file.close();
		QFile file1(fullFileName);
		if (file1.exists()) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool CommonTool_FileOperator::removeFile(QString fullFileName)
{
	try {
		if (!isFileExist(fullFileName)) {
			return false;
		}

		QFile fileTemp(fullFileName);
		fileTemp.remove();

		return true;
	}
	catch (const std::exception& ex) {
		return false;
	}
}

bool CommonTool_FileOperator::removeFiles(std::vector<QString> fullFileNames)
{
	try {
		bool res = true;
		for (auto item : fullFileNames) {
			res &= removeFile(item);
		}
		return res;
	}
	catch (...) {
		return false;
	}
}

bool CommonTool_FileOperator::removeFiles(QStringList lststrFullFileNames)
{
	try {
		bool res = true;
		for (auto item : lststrFullFileNames) {
			res &= removeFile(item);
		}
		return res;
	}
	catch (...) {
		return false;
	}
}

bool CommonTool_FileOperator::removeDirRec(QString fullPath)
{
	try {
		if (!isDirExist(fullPath)) {
			return false;
		}

		QDir dir(fullPath);
		dir.removeRecursively();

		return true;
	}
	catch (const std::exception& ex) {
		return false;
	}
}

bool CommonTool_FileOperator::removeFlatDirFiles(QString fullPath)
{
	try {
		vector<QString> file_name_list = getFileList(fullPath);
		for (auto item : file_name_list) {
			removeFile(item);
		}
		return true;
	}
	catch (const std::exception& ex) {
		return false;
	}
}

bool CommonTool_FileOperator::copyFile(QString fullFileName, QString newFullFileName)
{
	try {
		if (!isFileExist(fullFileName)) {
			return false;
		}
		if (isFileExist(newFullFileName)) {
			if (!removeFile(newFullFileName)) {
				return false;
			}
		}

		QFile::copy(fullFileName, newFullFileName);

		return true;
	}
	catch (const std::exception& ex) {
		return false;
	}
}

bool CommonTool_FileOperator::copyFiles(vector<QString> fullFileNames, QString newDir)
{
	try {
		bool res = true;
		if ((!newDir.endsWith("/")) &&
			(!newDir.endsWith("\\"))) {
			newDir += "/";
		}
		for (auto item : fullFileNames) {
			QFileInfo fileInfo(item);
			QString fileName = fileInfo.fileName();
			QString newFullFileName = newDir + fileName;
			if (isFileExist(newFullFileName)) {
				res &= removeFile(newFullFileName);
			}
			res &= copyFile(item, newFullFileName);
		}
		return res;
	}
	catch (...) {
		return false;
	}
}

bool CommonTool_FileOperator::moveFile(QString fullFileName, QString newFullFileName)
{
	try {
		if (!isFileExist(fullFileName)) {
			return false;
		}

		QFileInfo fi(newFullFileName);
		if (!isDirExist(fi.absoluteFilePath())) {
			createDir(fi.absoluteFilePath());
		}

		QFile::copy(fullFileName, newFullFileName);
		QFile::remove(fullFileName);

		return true;
	}
	catch (const std::exception& ex) {
		return false;
	}
}

bool CommonTool_FileOperator::moveFiles(vector<QString> fullFileNames, QString newDir)
{
	try {
		bool res = true;
		if ((!newDir.endsWith("/")) &&
			(!newDir.endsWith("\\"))) {
			newDir += "/";
		}
		for (auto item : fullFileNames) {
			QFileInfo fileInfo(item);
			QString fileName = fileInfo.fileName();
			QString newFullFileName = newDir + fileName;
			res &= moveFile(item, newFullFileName);
		}
		return res;
	}
	catch (...) {
		return false;
	}
}

vector<QString> CommonTool_FileOperator::getFileList(const QString& dir, bool isFullPath)
{
	vector<QString> file_path;
	QDir dir_path(dir);
	QList<QFileInfo> file(dir_path.entryInfoList());
	QString name = "";

	for (auto iter = file.begin(); iter != file.end(); iter++) {
		name = iter->fileName();
		if (isFullPath) {
			name = dir + "/" + name;
		}
		if (iter->isFile()) {
			file_path.push_back(name);
		}
	}

	return file_path;
}

vector<QString> CommonTool_FileOperator::getDirList(const QString& dir, bool isFullPath)
{
	vector<QString> res_dir_path;
	QDir dir_path(dir);
	QList<QFileInfo> file(dir_path.entryInfoList());
	QString name = "";

	for (auto iter = file.begin(); iter != file.end(); iter++) {
		name = iter->fileName();
		if (name == "." || name == "..") {
			continue;
		}
		if (isFullPath) {
			name = dir + "/" + name;
		}
		if (iter->isDir()) {
			res_dir_path.push_back(name);
		}
	}

	return res_dir_path;
}

void CommonTool_FileOperator::getDirList(const QString strDir, QList<QString>* plistDirPath, bool bIsFullPath)
{
	if (nullptr == plistDirPath)
	{
		return;
	}

	QDir dirPath(strDir);
	QList<QFileInfo> listFile(dirPath.entryInfoList());
	QString strName = "";

	for (auto iter = listFile.begin(); iter != listFile.end(); iter++) {
		strName = iter->fileName();
		if (strName == "." || strName == "..") {
			continue;
		}
		if (bIsFullPath) {
			strName = strDir + "/" + strName;
		}
		if (iter->isDir()) {
			plistDirPath->append(strName);
		}
	}
}

vector<QString> CommonTool_FileOperator::getDirFileList(const QString& dir, bool isFullPath)
{
	vector<QString> res_path;
	QDir dir_path(dir);
	QList<QFileInfo> file(dir_path.entryInfoList());
	QString name = "";

	for (auto iter = file.begin(); iter != file.end(); iter++) {
		name = iter->fileName();
		if (name == "." || name == "..") {
			continue;
		}
		if (isFullPath) {
			name = dir + "/" + name;
		}
		res_path.push_back(name);
	}

	return res_path;
}

vector<QString> CommonTool_FileOperator::getSpecialFileList(const QString& dir, const QString& specialName, int specialNameType, const vector<QString>& specialSuffix, bool isFullPath)
{
	// specialNameType : 0 -- special name any position
	//                   1 -- special name exists head position
	//                   2 -- special name exists tail position
	//                   otherwise -- special name filter no effect

	if (!isDirExist(dir)) {
		return vector<QString>();
	}

	vector<QString> file_path;
	QDir dir_path(dir);
	QList<QFileInfo> file(dir_path.entryInfoList());
	QString name = "";

	for (auto iter = file.begin(); iter != file.end(); iter++) {
		if (iter->isFile()) {
			name = iter->fileName();
			QStringList name_split = name.split(".");

			bool push_flag = true;
			if (specialSuffix.size() == 0) {
				if (name_split.size() == 2) {
					bool is_in = false;
					for (auto item : specialSuffix) {
						if (item == name_split.at(1)) {
							is_in = true;
							break;
						}
					}
					if (!is_in) {
						push_flag = false;
					}
				}
				else {
					push_flag = false;
				}
			}
			if (!push_flag) {
				continue;
			}
			if (specialNameType == 0) {
				if (!name_split.at(1).contains(specialName)) {
					push_flag = false;
				}
			}
			else if (specialNameType == 1) {
				if (name_split.at(1).size() >= specialName.size()) {
					if (name_split.at(1).left(specialName.size()) != specialName) {
						push_flag = false;
					}
				}
				else {
					push_flag = false;
				}
			}
			else if (specialNameType == 2) {
				if (name_split.at(1).size() >= specialName.size()) {
					if (name_split.at(1).right(specialName.size()) != specialName) {
						push_flag = false;
					}
				}
				else {
					push_flag = false;
				}
			}
			else {
				;
			}
			if (push_flag) {
				if (isFullPath) {
					name = dir + "/" + name;
				}
				file_path.push_back(name);
			}
		}
	}

	return file_path;
}

QFileInfoList CommonTool_FileOperator::getFileListByTraverseFolder(QString path)
{
	QFileInfoList file_list;
	QDir dir(path);
	file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	for (int i = 0; i < folder_list.size(); i++)
	{
		QString name = folder_list.at(i).absoluteFilePath();
		QFileInfoList child_file_list = getFileListByTraverseFolder(name);
		file_list.append(child_file_list);
	}
	return file_list;
}

void CommonTool_FileOperator::removeEmptyDirectoryByTraverseFolder(QString path)
{
	QDir dir(path);
	if (!dir.exists())
	{
		return;
	}
	QFileInfoList fileOrFolderList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);

	for (auto t : fileOrFolderList)
	{
		if (t.isFile())
		{
			continue;
		}
		else if (t.isDir())
		{
			QDir childDir(t.absoluteFilePath());
			QFileInfoList childFileOrFolderList = childDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
			if (childFileOrFolderList.count() <= 0)
			{
				childDir.rmdir(t.absoluteFilePath());
			}
			else
			{
				removeEmptyDirectoryByTraverseFolder(t.absoluteFilePath());
			}
		}
	}


	if (fileOrFolderList.count() <= 0)
	{
		dir.remove(path);
	}
	else
	{
		QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
		for (int i = 0; i < folder_list.size(); i++)
		{
			QString name = folder_list.at(i).absoluteFilePath();
			removeEmptyDirectoryByTraverseFolder(name);
		}
	}
}

bool CommonTool_FileOperator::sortFileList(vector<QString>& fileList)
{
	try {
		std::sort(fileList.begin(), fileList.end(), [](QString& s1, QString& s2) {return(s1 > s2); });
		return true;
	}
	catch (const std::exception& ex) {
		return false;
	}
}

bool CommonTool_FileOperator::readAllDataFromFile(QString file_name, vector<QString>& all_line_data)
{
	try {
		if (!isFileExist(file_name)) {
			return false;
		}

		QFile file(file_name);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			return false;
		}

		QTextStream file_str(&file);
		vector<QString> emptyVec;
		all_line_data.swap(emptyVec);

		while (!file_str.atEnd()) {
			all_line_data.push_back(file_str.readLine());
		}

		file.close();

		return true;
	}
	catch (const std::exception& ex) {
		return false;
	}
}

bool CommonTool_FileOperator::writeAllDataIntoFile(QString file_name, vector<QString>& all_line_data, bool is_supersede, bool is_add_line_feed)
{
	try {
		QFile file(file_name);
		if (is_supersede) {
			file.open(QIODevice::WriteOnly | QIODevice::Text);
		}
		else {
			file.open(QIODevice::Append | QIODevice::Text);
		}

		if (!file.isOpen()) {
			return false;
		}

		QTextStream file_str(&file);

		for (auto item : all_line_data) {
			file_str << item;
			if (is_add_line_feed) {
				file_str << "\n";
			}
		}

		file.close();

		return true;
	}
	catch (const std::exception& ex) {
		return false;
	}
}

QString CommonTool_FileOperator::getCurrentTimeStamp()
{
	QDate cur_date = QDate::currentDate();
	QTime cur_time;
	QString time_stamp = cur_date.toString(Qt::ISODate) + QString("-") + cur_time.currentTime().toString("hh:mm:ss.zzz");
	return time_stamp;
}

QString CommonTool_FileOperator::getCurrentTimeStamp1()
{
	QTime cur_time;
	return cur_time.currentTime().toString("hh:mm:ss.zzz");
}

QString CommonTool_FileOperator::getCurrentTimeStamp2()
{
	QDate cur_date = QDate::currentDate();
	QTime cur_time;
	QString time_stamp = cur_date.toString(Qt::ISODate) + QString("-") + cur_time.currentTime().toString("hhmmsszzz");
	return time_stamp;
}

QString CommonTool_FileOperator::getCurrentTimeStamp3()
{
	QDate cur_date = QDate::currentDate();
	QTime cur_time;
	QString time_stamp = cur_date.toString(Qt::ISODate) + cur_time.currentTime().toString("hhmmsszzz");
	return time_stamp;
}

QString CommonTool_FileOperator::getCurrentTimeStamp4()
{
	QTime cur_time;
	QString time_stamp = cur_time.currentTime().toString("hhmmsszzz");
	return time_stamp;
}

QString CommonTool_FileOperator::getCurrentDateStamp()
{
	QDate cur_date = QDate::currentDate();
	return cur_date.toString(Qt::ISODate);
}

QString CommonTool_FileOperator::getCurrentDateStamp1()
{
	return QDate::currentDate().toString("yyyyMMdd");
}

QStringList CommonTool_FileOperator::splitFilePathname(QString strFilePathname)
{
	try
	{
		QFileInfo qfiFileInfo(strFilePathname);

		QString strFilePath = "";
		QString strFileName = "";

		strFilePath = qfiFileInfo.absoluteFilePath() + "/";
		strFileName = qfiFileInfo.fileName();

		QStringList lststrFilePathAndName;
		lststrFilePathAndName.append(strFilePath);
		lststrFilePathAndName.append(strFileName);

		return lststrFilePathAndName;
	}
	catch (std::exception& ex)
	{
		return QStringList();
	}
}

QStringList CommonTool_FileOperator::splitFileName(QString strFileName)
{
	try
	{
		QFileInfo qfiFileInfo(strFileName);

		QString strFileBaseName = "";
		QString strFileSuffix = "";

		strFileBaseName = qfiFileInfo.completeBaseName();
		strFileSuffix = qfiFileInfo.suffix();

		QStringList lststrFileNameAndSuffix;
		lststrFileNameAndSuffix.append(strFileBaseName);
		lststrFileNameAndSuffix.append(strFileSuffix);

		return lststrFileNameAndSuffix;
	}
	catch (std::exception& ex)
	{
		return QStringList();
	}
}
