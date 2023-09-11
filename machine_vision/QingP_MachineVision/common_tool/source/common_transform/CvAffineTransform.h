#ifndef COMMONTOOL_COMMONTRANSFORM_CVAFFINETRANSFROM_H
#define COMMONTOOL_COMMONTRANSFORM_CVAFFINETRANSFROM_H

#include "../../common_tool_global.h"

#include <QObject>
#include <vector>

#include <opencv2/opencv.hpp>



class COMMON_TOOL_EXPORT CvAffineTransForm : public QObject
{
	Q_OBJECT
public:
	struct Rect4Vertices
	{
		bool is_ok = false;
		cv::Point2d top_left_point;
		cv::Point2d top_right_point;
		cv::Point2d bottom_left_point;
		cv::Point2d bottom_right_point;

		void clear() {
			is_ok = false;
		}
		void move(double trans_x, double trans_y) {
			if (is_ok) {
				top_left_point.x += trans_x;
				top_left_point.y += trans_y;
				top_right_point.x += trans_x;
				top_right_point.y += trans_y;
				bottom_left_point.x += trans_x;
				bottom_left_point.y += trans_y;
				bottom_right_point.x += trans_x;
				bottom_right_point.y += trans_y;
			}
		}
	};

	static bool rotateVector(float vector_x, float vector_y, float angle, float& new_vector_x, float& new_vector_y);

	static cv::Point2f getVerticalCrossPointOfLinePoint(cv::Vec4f* line, cv::Point2f p);

	static cv::Vec4f getVerticalLineOfLine(cv::Vec4f* line, cv::Point2f p /*= Point2f(-1, -1)*/);

	static void getCrossPoint(cv::Vec4f* l1, cv::Vec4f* l2, cv::Point2f* p);

	static void getCrossPoint(cv::Vec4f* l1, cv::Vec4f* l2, cv::Point2d* p);

	static void sortRectFourPoints(std::vector<cv::Point2d>& vertices);

	static void rotatePointMat( cv::Mat MImg, cv::Mat &MRst , double dAngle, cv::Point2f pPoint);

	static void rotatePointByPoint(cv::Point2f pPoint, cv::Point2f &pRst, float fAngle, cv::Point2f pCenter );

	static void rotatePointByPoint(cv::Point pPoint, cv::Point& pRst, float fAngle, cv::Point pCenter);

	static void translatePoint(cv::Point pPoint, cv::Point& pRst, int iTranslate_x, int iTranslate_y);

	static void translatePoint(cv::Point pPoint, cv::Point& pRst, cv::Point transalte);

	static void translatePoint(cv::Point2f pPoint, cv::Point2f& pRst, float iTranslate_x, float iTranslate_y);

	static void translatePoint(cv::Point2f pPoint, cv::Point2f& pRst, cv::Point2f pTranslate);

	static void translateAndRotatePoint(cv::Point pPoint, cv::Point &pRst, int iTranslate_x, int iTranslate_y, int angle, cv::Point pCenter);

	static void translateAndRotatePoint(cv::Point pPoint, cv::Point& pRst, cv::Point pTranslate, int iAngle, cv::Point pCenter);
	
	static void translateAndRotatePoint(cv::Point2f pPoint, cv::Point2f& pRst, float iTranslate_x, float iTranslate_y, float fAngle, cv::Point2f pCenter);
	
	static void translateAndRotatePoint(cv::Point2f pPoint, cv::Point2f& pRst, cv::Point2f pTranslate, float fAngle, cv::Point2f pCenter);

	static void rotateAndTranslatePoint(cv::Point pPoint, cv::Point &pRst, int iAngle, cv::Point pCenter, int iTranslate_x, int iTranslate_y);

	static void rotateAndTranslatePoint(cv::Point pPoint, cv::Point& pRst, int iAngle, cv::Point pCenter, cv::Point pTranslate);

	static void rotateAndTranslatePoint(cv::Point2f pPoint, cv::Point2f &pRst, float fAngle, cv::Point2f pCenter, float iTranslate_x, float iTranslate_y);
	
	static void rotateAndTranslatePoint(cv::Point2f pPoint, cv::Point2f& pRst, float fAngle, cv::Point2f pCenter, cv::Point2f pTranslate);

	static void translateAndRotateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, cv::Point pTranslate, int iAngle, cv::Point pCenter);

	static void translateAndRotateRect(cv::Rect RcRect, cv::RotatedRect &RtRcRst, int iTranslate_x, int iTranslate_y, int iAngle, cv::Point pCenter);

	static void translateAndRotateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, cv::Point2f pTranslate, float fAngle, cv::Point2f pCenter);

	static void translateAndRotateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, float iTranslate_x, float iTranslate_y, float fAngle, cv::Point2f pCenter);

	static void translateAndRotateRectPV(cv::Rect RcRect, std::vector <cv::Point2d>&vpRst ,cv::Point pTranslate, int iAngle, cv::Point pCenter, bool is_sort);

	static void translateAndRotateRectPV(cv::Rect RcRect, std::vector <cv::Point2d>&vpRst, int iTranslate_x, int iTranslate_y, int iAangle, cv::Point pCenter, bool is_sort);

	static void translateAndRotateRectPV(cv::Rect RcRect, std::vector <cv::Point2d>& vpRst, cv::Point2f pTranslate, float iAngle, cv::Point2f pCenter, bool is_sort);

	static void translateAndRotateRectPV(cv::Rect RcRect, std::vector <cv::Point2d>& vpRst, float iTranslate_x, float iTranslate_y, float iAangle, cv::Point2f pCenter, bool is_sort);

	static void translateAndRotateR4V(Rect4Vertices r4v, Rect4Vertices &r4vRst, float fTranslate_x, float fTranslate_y, float fAngle, cv::Point2f pCenter);

	static void rotateAndTranslateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, int iAngle,cv::Point pCenter,  cv::Point pTranslate);

	static void rotateAndTranslateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, int iAngle, cv::Point pCenter, int iTranslate_x, int iTranslate_y);

	static void rotateAndTranslateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, float iAngle, cv::Point2f pCenter, cv::Point2f pTranslate);

	static void rotateAndTranslateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, float fAngle, cv::Point2f pCenter, float fTranslate_x, float fTranslate_y);

	static void rotateAndTranslateRectPV(cv::Rect RcRect, std::vector <cv::Point2d>& vpRst, cv::Point pTranslate, int iAngle, cv::Point pCenter, bool is_sort);

	static void rotateAndTranslateRectPV(cv::Rect RcRect, std::vector <cv::Point2d>& vpRst, int iTranslate_x, int iTranslate_y, int iAangle, cv::Point pCenter, bool is_sort);

	static void rotateAndTranslateRectPV(cv::Rect RcRect, std::vector <cv::Point2d>& vpRst, cv::Point2f pTranslate, float fAngle, cv::Point2f pCenter, bool is_sort);

	static void rotateAndTranslateRectPV(cv::Rect RcRect, std::vector <cv::Point2d>& vpRst, float fTranslate_x, float fTranslate_y, float fAangle, cv::Point2f pCenter, bool is_sort);

	static void rotateAndTranslateR4V(Rect4Vertices r4v, Rect4Vertices& r4vRst, float fAngle, cv::Point2f pCenter, float fTranslate_x, float fTranslate_y);

	static void affineImage(cv::Mat Img, std::vector<std::tuple<bool, cv::Point2d, double>> params, cv::Mat& Rst, QString& errMessage);

	static void affinePoint(cv::Point2d PPoint, std::vector<std::tuple<bool, cv::Point2d, double>> params, cv::Point2d& RstPoint, QString& errMessage);

	static void translateAndRotateLine(cv::Vec4f v4line, cv::Vec4f &v4Rst, float fTranslate_x, float fTranslate_y, float angle, cv::Point2f center);

	static void rotateAndTranslateLine(cv::Vec4f v4line, cv::Vec4f& v4Rst, float fAngle, cv::Point2f pCenter, float fTranslate_x, float fTranslate_y);

	static void mySobelWithDirect(cv::Mat img, cv::Mat &Rst, bool isX, bool isReverse);

	static void normalizeLineDir(cv::Vec4f& line);

	static void correctLineDir(cv::Vec4f& line);

	static void correctLineDirByX(cv::Vec4f& line, bool isXPositive);
	
	static void correctLineDirByY(cv::Vec4f& line, bool isYPositive);

	static void getLineAngle(double& angle, cv::Vec4f& line, bool isAngle = true);

	static void getLineAngle1(double& angle, cv::Vec4f& line, bool isAngle = true);

};

#endif 
