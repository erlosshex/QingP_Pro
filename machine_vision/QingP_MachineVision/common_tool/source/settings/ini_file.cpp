#include "ini_file.h"

CommonTool_IniFile::CommonTool_IniFile()
{

}

CommonTool_IniFile::~CommonTool_IniFile()
{
	CommonTool_NormalTool::deleteDataPtr(m_qsetptrIniFile);
}

void CommonTool_IniFile::open(const QString& iniFilePathname)
{
	CommonTool_NormalTool::deleteDataPtr(m_qsetptrIniFile);

	m_qsetptrIniFile = new QSettings(iniFilePathname, QSettings::IniFormat);

}

void CommonTool_IniFile::readIni(const QString& qstrSectionName, const QString& qstrKeyName, bool& iValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		iValue = m_qsetptrIniFile->value(qstrKeyName).toBool();

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::readIni(const QString& qstrSectionName, const QString& qstrKeyName, int& iValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		iValue = m_qsetptrIniFile->value(qstrKeyName).toInt();

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::readIni(const QString& qstrSectionName, const QString& qstrKeyName, double& dValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		dValue = m_qsetptrIniFile->value(qstrKeyName).toDouble();

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::readIni(const QString& qstrSectionName, const QString& qstrKeyName, QString& qstrValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		qstrValue = m_qsetptrIniFile->value(qstrKeyName).toString();

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::readIni(const QString& qstrSectionName, const QString& qstrKeyName, cv::Rect& cvrValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		QRect qrValue = m_qsetptrIniFile->value(qstrKeyName).toRect();

		cvrValue.x = qrValue.x();
		cvrValue.y = qrValue.y();
		cvrValue.width = qrValue.width();
		cvrValue.height = qrValue.height();

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::readIni(const QString& qstrSectionName, const QString& qstrKeyName, cv::Point& cvpiValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		QPoint qpiValue = m_qsetptrIniFile->value(qstrKeyName).toPoint();

		cvpiValue.x = qpiValue.x();
		cvpiValue.y = qpiValue.y();

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::readIni(const QString& qstrSectionName, const QString& qstrKeyName, cv::Point2d& cvpValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		QPointF qpValue = m_qsetptrIniFile->value(qstrKeyName).toPointF();

		cvpValue.x = qpValue.x();
		cvpValue.y = qpValue.y();

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::readIni(const QString& qstrSectionName, const QString& qstrKeyName, QVector<cv::Point>& qvecpiValue)
{
	try
	{
		qvecpiValue.clear();

		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		QVariantList qvlValue = m_qsetptrIniFile->value(qstrKeyName).toList();

		for (auto& item : qvlValue) qvecpiValue.append(cv::Point(item.toPoint().x(), item.toPoint().y()));

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::readIni(const QString& qstrSectionName, const QString& qstrKeyName, QVector<cv::Point2d>& qvecpValue)
{
	try
	{
		qvecpValue.clear();

		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		QVariantList qvlValue = m_qsetptrIniFile->value(qstrKeyName).toList();

		for (auto& item : qvlValue) qvecpValue.append(cv::Point(item.toPointF().x(), item.toPointF().y()));

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const bool& iValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		m_qsetptrIniFile->setValue(qstrKeyName, iValue);

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const int& iValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		m_qsetptrIniFile->setValue(qstrKeyName, iValue);

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const double& dValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		m_qsetptrIniFile->setValue(qstrKeyName, dValue);

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const QString& qstrValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		m_qsetptrIniFile->setValue(qstrKeyName, qstrValue);

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const cv::Rect& cvrValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		QRect qrRect;
		qrRect.setX(cvrValue.x);
		qrRect.setY(cvrValue.y);
		qrRect.setWidth(cvrValue.width);
		qrRect.setHeight(cvrValue.height);

		m_qsetptrIniFile->setValue(qstrKeyName, qrRect);

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const cv::Point& cvpiValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		QPoint qpiPoint;
		qpiPoint.setX(cvpiValue.x);
		qpiPoint.setY(cvpiValue.y);

		m_qsetptrIniFile->setValue(qstrKeyName, qpiPoint);

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const cv::Point2d& cvpValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		QPointF qpPoint;
		qpPoint.setX(cvpValue.x);
		qpPoint.setY(cvpValue.y);

		m_qsetptrIniFile->setValue(qstrKeyName, qpPoint);

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const QVector<cv::Point>& qvecpiValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		QVariantList qlistpiPoints;
		for (auto p : qvecpiValue) qlistpiPoints.append(QPoint(p.x, p.y));

		m_qsetptrIniFile->setValue(qstrKeyName, qlistpiPoints);

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::writeIni(const QString& qstrSectionName, const QString& qstrKeyName, const QVector<cv::Point2d>& qvecpValue)
{
	try
	{
		if (m_qsetptrIniFile == nullptr) return;

		m_qsetptrIniFile->beginGroup(qstrSectionName);

		QVariantList qlistpPoints;
		for (auto p : qvecpValue) qlistpPoints.append(QPointF(p.x, p.y));

		m_qsetptrIniFile->setValue(qstrKeyName, qlistpPoints);

		m_qsetptrIniFile->endGroup();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::getSectionNameList(QStringList& qstrlistSectionNameList)
{
	try
	{
		qstrlistSectionNameList.clear();

		qstrlistSectionNameList = m_qsetptrIniFile->childGroups();
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CommonTool_IniFile::getKeyNameList(const QString& qstrSectionName, QStringList& qstrlistKeyNameList)
{
	try
	{
		qstrlistKeyNameList.clear();

		if ("" == qstrSectionName)
		{
			qstrlistKeyNameList = m_qsetptrIniFile->childKeys();
		}
		else
		{
			m_qsetptrIniFile->beginGroup(qstrSectionName);

			qstrlistKeyNameList = m_qsetptrIniFile->childKeys();

			m_qsetptrIniFile->endGroup();
		}
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

bool CommonTool_IniFile::isSectionExist(const QString& strSectionName)
{
	QStringList strlstSectionNameList;
	getSectionNameList(strlstSectionNameList);
	return strlstSectionNameList.contains(strSectionName);
}

bool CommonTool_IniFile::isSectionKeyExist(const QString& strSectionName, const QString& strKeyName)
{
	if (false == isSectionExist(strSectionName))
	{
		return false;
	}

	QStringList strlstKeyNameList;
	getKeyNameList(strSectionName, strlstKeyNameList);
	return strlstKeyNameList.contains(strKeyName);
}
