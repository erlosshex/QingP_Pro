#pragma once
#include "../../common_tool_global.h"
#include "../normal_tool/NormalTool.h"

#include <QObject>
#include <QSettings>
#include <QRect>
#include <QPoint>
#include <QPointF>

#include <opencv2/opencv.hpp>

class COMMON_TOOL_EXPORT CommonTool_IniFile : public QObject
{
	Q_OBJECT
public:
	CommonTool_IniFile();
	~CommonTool_IniFile();

	void open(const QString& iniFilePathname);

#pragma region Read
	void readIni(const QString& qstrSectionName, const QString& qstrKeyName, bool& iValue);

	void readIni(const QString& qstrSectionName, const QString& qstrKeyName, int& iValue);

	void readIni(const QString& qstrSectionName, const QString& qstrKeyName, double& dValue);

	void readIni(const QString& qstrSectionName, const QString& qstrKeyName, QString& qstrValue);

	void readIni(const QString& qstrSectionName, const QString& qstrKeyName, cv::Rect& cvrValue);

	void readIni(const QString& qstrSectionName, const QString& qstrKeyName, cv::Point& cvpiValue);

	void readIni(const QString& qstrSectionName, const QString& qstrKeyName, cv::Point2d& cvpValue);

	void readIni(const QString& qstrSectionName, const QString& qstrKeyName, QVector<cv::Point>& qvecpiValue);

	void readIni(const QString& qstrSectionName, const QString& qstrKeyName, QVector<cv::Point2d>& qvecpValue);
#pragma endregion

#pragma region Write
	void writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const bool& iValue);

	void writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const int& iValue);

	void writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const double& dValue);

	void writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const QString& qstrValue);

	void writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const cv::Rect& cvrValue);

	void writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const cv::Point& cvpiValue);

	void writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const cv::Point2d& cvpValue);

	void writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const QVector<cv::Point>& qvecpiValue);

	void writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const QVector<cv::Point2d>& qvecpValue);
#pragma endregion

#pragma region other
	void getSectionNameList(QStringList& qstrlistSectionNameList);

	void getKeyNameList(const QString& qstrSectionName, QStringList& qstrlistKeyNameList);

	bool isSectionExist(const QString& strSectionName);

	bool isSectionKeyExist(const QString& strSectionName, const QString& strKeyName);
#pragma endregion

private:
	QSettings* m_qsetptrIniFile = nullptr;
};

