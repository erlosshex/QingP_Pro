#include "CvAffineTransForm.h"

using namespace cv;

bool CvAffineTransForm::rotateVector(float vector_x, float vector_y, float angle, float& new_vector_x, float& new_vector_y)
{
	try {
		double dAngle = angle * CV_PI / 180;

		new_vector_x = vector_x * cos(dAngle) - sin(dAngle) * vector_y;
		new_vector_y = vector_x * sin(dAngle) + cos(dAngle) * vector_y;

		return true;
	}
	catch (const std::exception& ex) {
		return false;
	}
}

void CvAffineTransForm::getCrossPoint(Vec4f* l1, Vec4f* l2, Point2f* p)
{
	double vx1 = (*l1)[0];
	double vy1 = (*l1)[1];
	double x1 = (*l1)[2];
	double y1 = (*l1)[3];

	double vx2 = (*l2)[0];
	double vy2 = (*l2)[1];
	double x2 = (*l2)[2];
	double y2 = (*l2)[3];

	double operation = vx1 * vy2 - vx2 * vy1;

	if (operation != 0.0) {
		double x = (x2 * vx1 * vy2 - y2 * vx1 * vx2 - x1 * vx2 * vy1 + y1 * vx1 * vx2) / operation;
		double y = (x2 * vy1 * vy2 - y2 * vx2 * vy1 - x1 * vy1 * vy2 + y1 * vx1 * vy2) / operation;

		p->x = x;
		p->y = y;
	}
	else {
		p->x = -1;
		p->y = -1;
	}
}

void CvAffineTransForm::getCrossPoint(Vec4f* l1, Vec4f* l2, Point2d* p)
{
	double vx1 = (*l1)[0];
	double vy1 = (*l1)[1];
	double x1 = (*l1)[2];
	double y1 = (*l1)[3];

	double vx2 = (*l2)[0];
	double vy2 = (*l2)[1];
	double x2 = (*l2)[2];
	double y2 = (*l2)[3];

	double operation = vx1 * vy2 - vx2 * vy1;

	if (operation != 0.0) {
		double x = (x2 * vx1 * vy2 - y2 * vx1 * vx2 - x1 * vx2 * vy1 + y1 * vx1 * vx2) / operation;
		double y = (x2 * vy1 * vy2 - y2 * vx2 * vy1 - x1 * vy1 * vy2 + y1 * vx1 * vy2) / operation;

		p->x = x;
		p->y = y;
	}
	else {
		p->x = -1;
		p->y = -1;
	}
}

cv::Point2f CvAffineTransForm::getVerticalCrossPointOfLinePoint(cv::Vec4f* line, cv::Point2f p)
{
	Point2f res;
	Vec4f vertical_line = getVerticalLineOfLine(line, p);
	getCrossPoint(line, &vertical_line, &res);
	return res;
}

cv::Vec4f CvAffineTransForm::getVerticalLineOfLine(Vec4f* line, Point2f p /*= Point2f(-1, -1)*/)
{
	Vec4f res = (*line);
	res[0] = (*line)[1];
	res[1] = -((*line)[0]);

	if (res[0] < 0) {
		res[0] = -res[0];
		res[1] = -res[1];
	}

	if (p != Point2f(-1, -1)) {
		res[2] = p.x;
		res[3] = p.y;
	}

	return res;
}

void CvAffineTransForm::sortRectFourPoints(std::vector<cv::Point2d>& vertices)
{
	Point2d tempVertices[4];
	double verticesMeanX = 0;
	double verticesMeanY = 0;
	for (int i = 0; i < 4; ++i) {
		tempVertices[i] = vertices[i];
		verticesMeanX += vertices[i].x;
		verticesMeanY += vertices[i].y;
	}

	verticesMeanX = verticesMeanX / 4;
	verticesMeanY = verticesMeanY / 4;

	for (int i = 0; i < 4; ++i) {
		if (tempVertices[i].x<verticesMeanX && tempVertices[i].y>verticesMeanY) {
			vertices[0] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x < verticesMeanX && tempVertices[i].y < verticesMeanY) {
			vertices[1] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x > verticesMeanX && tempVertices[i].y < verticesMeanY) {
			vertices[2] = tempVertices[i];
			continue;
		}
		if (tempVertices[i].x > verticesMeanX && tempVertices[i].y > verticesMeanY) {
			vertices[3] = tempVertices[i];
			continue;
		}
	}
}

void CvAffineTransForm::rotatePointMat(cv::Mat MImg, cv::Mat& MRst, double dAngle, cv::Point2f pPoint)
{
	if (MImg.empty()) 
		return;

	if (dAngle == 0) 
		return;

	//将Mat  绕某点旋转任意角度 返回mat
	Mat MArray = cv::getRotationMatrix2D(pPoint, dAngle, 1);
	warpAffine(MImg, MRst, MArray, MRst.size());
}

void CvAffineTransForm::rotatePointByPoint(cv::Point2f pPoint, cv::Point2f& pRst, float fAngle, cv::Point2f pCenter)
{
	if (fAngle == 0)
		return;

	double huAngle = fAngle * CV_PI / 180.0;

	pRst.x = (pPoint.x - pCenter.x) * cos(huAngle) - (pPoint.y - pCenter.y) * sin(huAngle) + pCenter.x;
	pRst.y = (pPoint.x - pCenter.x) * sin(huAngle) + (pPoint.y - pCenter.y) * cos(huAngle) + pCenter.y;

}

void CvAffineTransForm::rotatePointByPoint(cv::Point pPoint, cv::Point& pRst, float fAngle, cv::Point pCenter)
{
	if (fAngle == 0)
		return;

	double huAngle = fAngle * CV_PI / 180.0;

	pRst.x = (pPoint.x - pCenter.x) * cos(huAngle) - (pPoint.y - pCenter.y) * sin(huAngle) + pCenter.x;
	pRst.y = (pPoint.x - pCenter.x) * sin(huAngle) + (pPoint.y - pCenter.y) * cos(huAngle) + pCenter.y;

}

void CvAffineTransForm::translatePoint(cv::Point pPoint, cv::Point &pRst, int iTranslate_x, int iTranslate_y)
{
	pRst.x = pPoint.x + iTranslate_x;
	pRst.y = pPoint.y + iTranslate_y;
}

void CvAffineTransForm::translatePoint(cv::Point pPoint,cv::Point &pRst, cv::Point pTranslate)
{
	translatePoint(pPoint, pRst, pTranslate.x, pTranslate.y);
}

void CvAffineTransForm::translatePoint(cv::Point2f pPoint, cv::Point2f& pRst, float iTranslate_x, float iTranslate_y)
{
	pRst.x = pPoint.x + iTranslate_x;
	pRst.y = pPoint.y + iTranslate_y;
}

void CvAffineTransForm::translatePoint(cv::Point2f pPoint, cv::Point2f& pRst, cv::Point2f pTranslate)
{
	translatePoint(pPoint, pRst, pTranslate.x, pTranslate.y);
}

void CvAffineTransForm::translateAndRotatePoint(cv::Point pPoint, cv::Point& pRst, int iTranslate_x, int iTranslate_y, int iAngle, cv::Point pCenter)
{
	if (iTranslate_x == 0 && iTranslate_y == 0 && iAngle == 0) {
		return;
	}

	translatePoint(pPoint, pRst, iTranslate_x, iTranslate_y);
	rotatePointByPoint(pRst, pRst, iAngle, pCenter);
}

void CvAffineTransForm::translateAndRotatePoint(cv::Point pPoint, cv::Point& pRst, cv::Point pTranslate, int iAngle, cv::Point pCenter)
{
	if (pTranslate.x == 0 && pTranslate.y == 0 && iAngle == 0) 
		return;

	 translatePoint(pPoint, pRst, pTranslate);
	 rotatePointByPoint(pRst, pRst, iAngle, pCenter);

}

void CvAffineTransForm::translateAndRotatePoint(cv::Point2f pPoint, cv::Point2f& pRst, float fTranslate_x, float fTranslate_y, float fAngle, cv::Point2f pCenter)
{
	if (fTranslate_x == 0 && fTranslate_y == 0 && fAngle == 0)
		return;

	translatePoint(pPoint, pRst, fTranslate_x, fTranslate_y);
	rotatePointByPoint(pRst, pRst, fAngle, pCenter);
}

void CvAffineTransForm::translateAndRotatePoint(cv::Point2f pPoint, cv::Point2f& pRst, cv::Point2f pTranslate, float fAngle, cv::Point2f pCenter)
{
	if (pTranslate.x == 0 && pTranslate.y == 0 && fAngle == 0)
		return;

	translatePoint(pPoint, pRst, pTranslate);
	rotatePointByPoint(pRst, pRst, fAngle, pCenter);

}

void CvAffineTransForm::rotateAndTranslatePoint(cv::Point pPoint, cv::Point& pRst, int iAngle, cv::Point pCenter, int iTranslate_x, int iTranslate_y)
{
	if (iTranslate_x == 0 && iTranslate_y == 0 && iAngle == 0) {
		return;
	}

	rotatePointByPoint(pRst, pRst, iAngle, pCenter);
	translatePoint(pPoint, pRst, iTranslate_x, iTranslate_y);
}

void CvAffineTransForm::rotateAndTranslatePoint(cv::Point pPoint, cv::Point& pRst, int iAngle, cv::Point pCenter, cv::Point pTranslate)
{
	if (pTranslate.x == 0 && pTranslate.y == 0 && iAngle == 0)
		return;

	rotatePointByPoint(pRst, pRst, iAngle, pCenter);
	translatePoint(pPoint, pRst, pTranslate);

}

void CvAffineTransForm::rotateAndTranslatePoint(cv::Point2f pPoint, cv::Point2f& pRst, float fAngle, cv::Point2f pCenter, float iTranslate_x, float iTranslate_y)
{
	if (iTranslate_x == 0 && iTranslate_y == 0 && fAngle == 0)
		return;

	rotatePointByPoint(pRst, pRst, fAngle, pCenter);
	translatePoint(pPoint, pRst, iTranslate_x, iTranslate_y);

}

void CvAffineTransForm::rotateAndTranslatePoint(cv::Point2f pPoint, cv::Point2f& pRst, float fAngle, cv::Point2f pCenter, cv::Point2f pTranslate)
{
	if (pTranslate.x == 0 && pTranslate.y == 0 && fAngle == 0)
		return;

	rotatePointByPoint(pRst, pRst, fAngle, pCenter);
	translatePoint(pPoint, pRst, pTranslate);

}

void CvAffineTransForm::translateAndRotateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, cv::Point pTranslate, int iAngle, cv::Point pCenter)
{
	translateAndRotateRect(RcRect, RtRcRst, pTranslate.x, pTranslate.y, iAngle, pCenter);
}

void CvAffineTransForm::translateAndRotateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, int iTranslate_x, int iTranslate_y, int iAngle, cv::Point pCenter)
{
	try {
		Point2f center_f = pCenter;
		float angle_f = iAngle;
		float translate_x_f = iTranslate_x;
		float translate_y_f = iTranslate_y;

		int rect_x = RcRect.x;
		int rect_y = RcRect.y;
		int rect_w = RcRect.width;
		int rect_h = RcRect.height;

		Point2f p1, p2, p3;

		p1.x = rect_x;
		p1.y = rect_y;

		p2.x = rect_x + rect_w - 1;
		p2.y = rect_y;

		p3.x = rect_x;
		p3.y = rect_y + rect_h - 1;

		Point2f new_p1, new_p2, new_p3;

		translateAndRotatePoint(p1, new_p1, translate_x_f, translate_y_f, angle_f, center_f);
		translateAndRotatePoint(p2, new_p2, translate_x_f, translate_y_f, angle_f, center_f);
		translateAndRotatePoint(p3, new_p3, translate_x_f, translate_y_f, angle_f, center_f);

		//cout << "p1 x : " << p1.x << ", y : " << p1.y << " -----> " << "new_p1 x : " << new_p1.x << ", y : " << new_p1.y << endl;
		//cout << "p2 x : " << p2.x << ", y : " << p2.y << " -----> " << "new_p2 x : " << new_p2.x << ", y : " << new_p2.y << endl;
		//cout << "p3 x : " << p3.x << ", y : " << p3.y << " -----> " << "new_p3 x : " << new_p3.x << ", y : " << new_p3.y << endl;

		Point2f new_p1_float = new_p1;
		Point2f new_p2_float = new_p2;
		Point2f new_p3_float = new_p3;

		RotatedRect new_rect(new_p3_float, new_p1_float, new_p2_float);

		RtRcRst = new_rect;
	}
	catch (std::exception& e) {
		RtRcRst = RotatedRect();
	}

}

void CvAffineTransForm::translateAndRotateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, cv::Point2f pTranslate, float fAngle, cv::Point2f pCenter)
{
	translateAndRotateRect(RcRect, RtRcRst, pTranslate.x, pTranslate.y, fAngle, pCenter);
}

void CvAffineTransForm::translateAndRotateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, float iTranslate_x, float iTranslate_y, float fAngle, cv::Point2f pCenter)
{
	try {
		int rect_x = RcRect.x;
		int rect_y = RcRect.y;
		int rect_w = RcRect.width;
		int rect_h = RcRect.height;

		Point2f p1, p2, p3;

		p1.x = rect_x;
		p1.y = rect_y;

		p2.x = rect_x + rect_w - 1;
		p2.y = rect_y;

		p3.x = rect_x;
		p3.y = rect_y + rect_h - 1;

		Point new_p1, new_p2, new_p3;

		translateAndRotatePoint(p1, new_p1, iTranslate_x, iTranslate_y, fAngle, pCenter);
		translateAndRotatePoint(p2, new_p2, iTranslate_x, iTranslate_y, fAngle, pCenter);
		translateAndRotatePoint(p3, new_p3, iTranslate_x, iTranslate_y, fAngle, pCenter);

		RotatedRect new_rect(new_p3, new_p1, new_p2);

		RtRcRst = new_rect;
	}
	catch (std::exception& e) {
		RtRcRst = RotatedRect();
	}
}

void CvAffineTransForm::translateAndRotateRectPV(cv::Rect RcRect, std::vector <cv::Point2d> &vpRst, cv::Point pTranslate, int iAngle, cv::Point pCenter, bool is_sort = true)
{
	translateAndRotateRectPV(RcRect, vpRst, pTranslate.x, pTranslate.y, iAngle, pCenter, is_sort);
}

void CvAffineTransForm::translateAndRotateRectPV(cv::Rect rect, std::vector <cv::Point2d>&vpRst, int iTranslate_x, int iTranslate_y , int iAangle, cv::Point pCenter, bool is_sort = true)
{
	try {
		Point2d p0;
		Point2d p1;
		Point2d p2;
		Point2d p3;

		p0.x = rect.x;
		p0.y = rect.y + rect.height - 1;
		p1.x = rect.x;
		p1.y = rect.y;
		p2.x = rect.x + rect.width - 1;
		p2.y = rect.y;
		p3.x = rect.x + rect.width - 1;
		p3.y = rect.y + rect.height - 1;

		Point2f new_p0;
		Point2f new_p1;
		Point2f new_p2;
		Point2f new_p3;

		translateAndRotatePoint(p0, new_p0, iTranslate_x, iTranslate_y, iAangle, pCenter);
		translateAndRotatePoint(p1, new_p1, iTranslate_x, iTranslate_y, iAangle, pCenter);
		translateAndRotatePoint(p2, new_p2, iTranslate_x, iTranslate_y, iAangle, pCenter);
		translateAndRotatePoint(p3, new_p3, iTranslate_x, iTranslate_y, iAangle, pCenter);

		new_p0=Point2d(new_p0);
		new_p1=Point2d(new_p1);
		new_p2=Point2d(new_p2);
		new_p3=Point2d(new_p3);

		vpRst.push_back(new_p0);
		vpRst.push_back(new_p1);
		vpRst.push_back(new_p2);
		vpRst.push_back(new_p3);

		if (is_sort) {
			sortRectFourPoints(vpRst);
		}
	}
	catch (const std::exception& ex) {
		vpRst.clear();
	}
}

void CvAffineTransForm::translateAndRotateRectPV(cv::Rect RcRect, std::vector <cv::Point2d>& vpRst, cv::Point2f pTranslate, float iAngle, cv::Point2f pCenter, bool is_sort = true)
{
	translateAndRotateRectPV(RcRect, vpRst, pTranslate.x,pTranslate.y, iAngle, pCenter, is_sort);
}

void CvAffineTransForm::translateAndRotateRectPV(cv::Rect RcRect, std::vector <cv::Point2d>& vpRst, float iTranslate_x, float iTranslate_y, float iAangle, cv::Point2f pCenter, bool is_sort = true)
{
	try {
		Point2d p0;
		Point2d p1;
		Point2d p2;
		Point2d p3;

		p0.x = RcRect.x;
		p0.y = RcRect.y + RcRect.height - 1;
		p1.x = RcRect.x;
		p1.y = RcRect.y;
		p2.x = RcRect.x + RcRect.width - 1;
		p2.y = RcRect.y;
		p3.x = RcRect.x + RcRect.width - 1;
		p3.y = RcRect.y + RcRect.height - 1;

		Point2f new_p0;
		Point2f new_p1;
		Point2f new_p2;
		Point2f new_p3;

		translateAndRotatePoint(p0, new_p0, iTranslate_x, iTranslate_y, iAangle, pCenter);
		translateAndRotatePoint(p1, new_p1, iTranslate_x, iTranslate_y, iAangle, pCenter);
		translateAndRotatePoint(p2, new_p2, iTranslate_x, iTranslate_y, iAangle, pCenter);
	    translateAndRotatePoint(p3, new_p3, iTranslate_x, iTranslate_y, iAangle, pCenter);

		new_p0=Point2d(new_p0);
		new_p1=Point2d(new_p1);
		new_p2=Point2d(new_p2);
		new_p3=Point2d(new_p3);

		vpRst.push_back(new_p0);
		vpRst.push_back(new_p1);
		vpRst.push_back(new_p2);
		vpRst.push_back(new_p3);

		if (is_sort) {
			sortRectFourPoints(vpRst);
		}
	}
	catch (const std::exception& ex) {
		vpRst.clear();
	}
}

void CvAffineTransForm::translateAndRotateR4V(Rect4Vertices r4v, Rect4Vertices& r4vRst , float fTranslate_x, float fTranslate_y, float fAngle, cv::Point2f pCenter)
{
	try {
		if (r4v.is_ok) 
		{
			r4vRst.is_ok = true;
			Point2f p1, p2, p3, p4;

			translateAndRotatePoint(Point2f(r4v.top_left_point.x, r4v.top_left_point.y),p1, fTranslate_x, fTranslate_y, fAngle, pCenter);
			translateAndRotatePoint(Point2f(r4v.top_right_point.x, r4v.top_right_point.y),p2,  fTranslate_x, fTranslate_y, fAngle, pCenter);
			translateAndRotatePoint(Point2f(r4v.bottom_left_point.x, r4v.bottom_left_point.y),p3, fTranslate_x, fTranslate_y, fAngle, pCenter);
			translateAndRotatePoint(Point2f(r4v.bottom_right_point.x, r4v.bottom_right_point.y),p4,  fTranslate_x, fTranslate_y, fAngle, pCenter);

			r4vRst.top_left_point = Point2d(p1);
			r4vRst.top_right_point = Point2d(p2);
			r4vRst.bottom_left_point = Point2d(p3);
			r4vRst.bottom_right_point = Point2d(p4);
		}
		else 
		{
			r4vRst.clear();
		}
	}
	catch (const std::exception& ex)
	{
		r4vRst = Rect4Vertices();
	}
}

void CvAffineTransForm::rotateAndTranslateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, int iAngle, cv::Point pCenter, cv::Point pTranslate)
{
	rotateAndTranslateRect(RcRect, RtRcRst, iAngle, pCenter, pTranslate.x, pTranslate.y);
}

void CvAffineTransForm::rotateAndTranslateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, int iAngle, cv::Point pCenter, int iTranslate_x, int iTranslate_y)
{
	try
	{
		Point2f center_f = pCenter;
		float angle_f = iAngle;
		float translate_x_f = iTranslate_x;
		float translate_y_f = iTranslate_y;

		int rect_x = RcRect.x;
		int rect_y = RcRect.y;
		int rect_w = RcRect.width;
		int rect_h = RcRect.height;

		Point2f p1, p2, p3;

		p1.x = rect_x;
		p1.y = rect_y;

		p2.x = rect_x + rect_w - 1;
		p2.y = rect_y;

		p3.x = rect_x;
		p3.y = rect_y + rect_h - 1;

		Point2f new_p1, new_p2, new_p3;

		rotateAndTranslatePoint(p1, new_p1, angle_f, center_f, translate_x_f, translate_y_f);
		rotateAndTranslatePoint(p2, new_p2, angle_f, center_f, translate_x_f, translate_y_f);
		rotateAndTranslatePoint(p3, new_p3, angle_f, center_f, translate_x_f, translate_y_f);

		//cout << "p1 x : " << p1.x << ", y : " << p1.y << " -----> " << "new_p1 x : " << new_p1.x << ", y : " << new_p1.y << endl;
		//cout << "p2 x : " << p2.x << ", y : " << p2.y << " -----> " << "new_p2 x : " << new_p2.x << ", y : " << new_p2.y << endl;
		//cout << "p3 x : " << p3.x << ", y : " << p3.y << " -----> " << "new_p3 x : " << new_p3.x << ", y : " << new_p3.y << endl;

		Point2f new_p1_float = new_p1;
		Point2f new_p2_float = new_p2;
		Point2f new_p3_float = new_p3;

		RotatedRect new_rect(new_p3_float, new_p1_float, new_p2_float);

		RtRcRst = new_rect;
	}
	catch (std::exception& e) {
		RtRcRst = RotatedRect();
	}
}

void CvAffineTransForm::rotateAndTranslateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, float iAngle, cv::Point2f pCenter, cv::Point2f pTranslate)
{
	rotateAndTranslateRect(RcRect, RtRcRst, iAngle, pCenter, pTranslate.x, pTranslate.y);
}

void CvAffineTransForm::rotateAndTranslateRect(cv::Rect RcRect, cv::RotatedRect& RtRcRst, float fAngle, cv::Point2f pCenter, float fTranslate_x, float fTranslate_y)
{
	try 
	{
		Point2f center_f = pCenter;
		float angle_f = fAngle;
		float translate_x_f = fTranslate_x;
		float translate_y_f = fTranslate_y;

		int rect_x = RcRect.x;
		int rect_y = RcRect.y;
		int rect_w = RcRect.width;
		int rect_h = RcRect.height;

		Point2f p1, p2, p3;

		p1.x = rect_x;
		p1.y = rect_y;

		p2.x = rect_x + rect_w - 1;
		p2.y = rect_y;

		p3.x = rect_x;
		p3.y = rect_y + rect_h - 1;

		Point2f new_p1, new_p2, new_p3;

		rotateAndTranslatePoint(p1, new_p1, angle_f, center_f, translate_x_f, translate_y_f);
		rotateAndTranslatePoint(p2, new_p2, angle_f, center_f, translate_x_f, translate_y_f);
		rotateAndTranslatePoint(p3, new_p3, angle_f, center_f, translate_x_f, translate_y_f);

		//cout << "p1 x : " << p1.x << ", y : " << p1.y << " -----> " << "new_p1 x : " << new_p1.x << ", y : " << new_p1.y << endl;
		//cout << "p2 x : " << p2.x << ", y : " << p2.y << " -----> " << "new_p2 x : " << new_p2.x << ", y : " << new_p2.y << endl;
		//cout << "p3 x : " << p3.x << ", y : " << p3.y << " -----> " << "new_p3 x : " << new_p3.x << ", y : " << new_p3.y << endl;

		Point2f new_p1_float = new_p1;
		Point2f new_p2_float = new_p2;
		Point2f new_p3_float = new_p3;

		RotatedRect new_rect(new_p3_float, new_p1_float, new_p2_float);

		RtRcRst = new_rect;
	}
	catch (std::exception& e) {
		RtRcRst = RotatedRect();
	}
}

void CvAffineTransForm::rotateAndTranslateRectPV(cv::Rect RcRect, std::vector<cv::Point2d>& vpRst, cv::Point pTranslate, int iAngle, cv::Point pCenter, bool is_sort)
{
	rotateAndTranslateRectPV( RcRect, vpRst, pTranslate.x, pTranslate.y, iAngle, pCenter, is_sort);
}

void CvAffineTransForm::rotateAndTranslateRectPV(cv::Rect RcRect, std::vector<cv::Point2d>&vpRst, int iTranslate_x, int iTranslate_y, int iAangle, cv::Point pCenter, bool is_sort)
{
	try {
		Point2d p0;
		Point2d p1;
		Point2d p2;
		Point2d p3;

		p0.x = RcRect.x;
		p0.y = RcRect.y + RcRect.height - 1;
		p1.x = RcRect.x;
		p1.y = RcRect.y;
		p2.x = RcRect.x + RcRect.width - 1;
		p2.y = RcRect.y;
		p3.x = RcRect.x + RcRect.width - 1;
		p3.y = RcRect.y + RcRect.height - 1;

		Point2f new_p0;
		Point2f new_p1;
		Point2f new_p2;
		Point2f new_p3;

		rotateAndTranslatePoint(p0, new_p0,  iAangle, pCenter,iTranslate_x, iTranslate_y);
		rotateAndTranslatePoint(p1, new_p1,  iAangle, pCenter,iTranslate_x, iTranslate_y);
		rotateAndTranslatePoint(p2, new_p2,  iAangle, pCenter,iTranslate_x, iTranslate_y);
		rotateAndTranslatePoint(p3, new_p3,  iAangle, pCenter,iTranslate_x, iTranslate_y );

		vpRst.push_back(new_p0);
		vpRst.push_back(new_p1);
		vpRst.push_back(new_p2);
		vpRst.push_back(new_p3);

		if (is_sort) {
			sortRectFourPoints(vpRst);
		}
	}
	catch (const std::exception& ex) {
		vpRst.clear();
	}
}

void CvAffineTransForm::rotateAndTranslateRectPV(cv::Rect RcRect, std::vector <cv::Point2d>& vpRst, cv::Point2f pTranslate, float fAngle, cv::Point2f pCenter, bool is_sort)
{
	rotateAndTranslateRectPV(RcRect, vpRst, pTranslate.x, pTranslate.y, fAngle, pCenter, is_sort);
}

void CvAffineTransForm::rotateAndTranslateRectPV(cv::Rect RcRect, std::vector <cv::Point2d>& vpRst, float fTranslate_x, float fTranslate_y, float fAangle, cv::Point2f pCenter, bool is_sort)
{
	try {
		Point2d p0;
		Point2d p1;
		Point2d p2;
		Point2d p3;

		p0.x = RcRect.x;
		p0.y = RcRect.y + RcRect.height - 1;
		p1.x = RcRect.x;
		p1.y = RcRect.y;
		p2.x = RcRect.x + RcRect.width - 1;
		p2.y = RcRect.y;
		p3.x = RcRect.x + RcRect.width - 1;
		p3.y = RcRect.y + RcRect.height - 1;

		Point2f new_p0;
		Point2f new_p1;
		Point2f new_p2;
		Point2f new_p3;

		rotateAndTranslatePoint(p0, new_p0, fAangle, pCenter, fTranslate_x, fTranslate_y);
		rotateAndTranslatePoint(p1, new_p1, fAangle, pCenter, fTranslate_x, fTranslate_y);
		rotateAndTranslatePoint(p2, new_p2, fAangle, pCenter, fTranslate_x, fTranslate_y);
		rotateAndTranslatePoint(p3, new_p3, fAangle, pCenter, fTranslate_x, fTranslate_y);

		vpRst.push_back(new_p0);
		vpRst.push_back(new_p1);
		vpRst.push_back(new_p2);
		vpRst.push_back(new_p3);

		if (is_sort) {
			sortRectFourPoints(vpRst);
		}
	}
	catch (const std::exception& ex) {
		vpRst.clear();
	}
}

void CvAffineTransForm::rotateAndTranslateR4V(Rect4Vertices r4v, Rect4Vertices& r4vRst, float fAngle, cv::Point2f pCenter, float fTranslate_x, float fTranslate_y)
{
	try {
		if (r4v.is_ok) {
			r4vRst.is_ok = true;
			Point2f p1, p2, p3, p4;
			rotateAndTranslatePoint(Point2f(r4v.top_left_point.x, r4v.top_left_point.y), p1, fAngle, pCenter, fTranslate_x, fTranslate_y);
			rotateAndTranslatePoint(Point2f(r4v.top_right_point.x, r4v.top_right_point.y), p2, fAngle, pCenter, fTranslate_x, fTranslate_y);
			rotateAndTranslatePoint(Point2f(r4v.bottom_left_point.x, r4v.bottom_left_point.y), p3, fAngle, pCenter, fTranslate_x, fTranslate_y);
			rotateAndTranslatePoint(Point2f(r4v.bottom_right_point.x, r4v.bottom_right_point.y), p4, fAngle, pCenter, fTranslate_x, fTranslate_y);

			r4vRst.top_left_point = Point2d(p1);
			r4vRst.top_right_point = Point2d(p2);
			r4vRst.bottom_left_point = Point2d(p3);
			r4vRst.bottom_right_point = Point2d(p4);
		}
		else {
			r4vRst.clear();
		}
	}
	catch (const std::exception& ex) {
		r4vRst = Rect4Vertices();
	}
}

void CvAffineTransForm::affineImage(cv::Mat Img, std::vector<std::tuple<bool, cv::Point2d, double>> params, cv::Mat& Rst, QString& errMessage)
{
	try {
		cv::Mat affineMat = cv::Mat::eye(3, 3, CV_64FC1);

		for (auto item : params) {
			if (std::get<0>(item)) {
				auto rotateMat = cv::getRotationMatrix2D(cv::Point2f(std::get<1>(item).x, std::get<1>(item).y), std::get<2>(item), 1);
				cv::Mat rotateMat1 = cv::Mat::eye(3, 3, CV_64FC1);
				for (int r = 0; r < 2; ++r) {
					for (int c = 0; c < 3; ++c) {
						rotateMat1.at<double>(r, c) = rotateMat.at<double>(r, c);
					}
				}
				affineMat = rotateMat1 * affineMat;
			}
			else {
				cv::Mat translateMat = cv::Mat::eye(3, 3, CV_64FC1);
				translateMat.at<double>(0, 2) = std::get<1>(item).x;
				translateMat.at<double>(1, 2) = std::get<1>(item).y;
				affineMat = translateMat * affineMat;
			}
		}

		cv::Mat affineMat1 = cv::Mat::eye(2, 3, CV_64FC1);

		for (int r = 0; r < 2; ++r) {
			for (int c = 0; c < 3; ++c) {
				affineMat1.at<double>(r, c) = affineMat.at<double>(r, c);
			}
		}

		cv::warpAffine(Img, Rst, affineMat1, Img.size());

		return;
	}
	catch (const std::exception& ex) {
		errMessage = ex.what();
		return;
	}
	catch (...) {
		errMessage = "Unknow Error";
		return;
	}
}

void CvAffineTransForm::affinePoint(cv::Point2d PPoint, std::vector<std::tuple<bool, cv::Point2d, double>> params, cv::Point2d& RstPoint, QString& errMessage)
{
	{
		try {
			cv::Point2f temp;
			temp.x = PPoint.x;
			temp.y = PPoint.y;

			for (auto item : params) {
				if (std::get<0>(item)) {
					rotatePointByPoint(temp, temp, (float)std::get<2>(item), cv::Point2f(std::get<1>(item).x, std::get<1>(item).y));
				}
				else {
					translatePoint(temp, temp, cv::Point2f(std::get<1>(item).x, std::get<1>(item).y));
				}
			}

			RstPoint.x = temp.x;
			RstPoint.y = temp.y;

			return;
		}
		catch (const std::exception& ex) {
			errMessage = ex.what();
			return;
		}
		catch (...) {
			errMessage = "Unknow Error";
			return;
		}
	}
}

void CvAffineTransForm::translateAndRotateLine(cv::Vec4f v4line, cv::Vec4f &v4Rst, float fTranslate_x, float fTranslate_y, float fAngle, cv::Point2f pCenter)
{
	try {
		float vector_x, vector_y;
		rotateVector(v4line[0], v4line[1], fAngle, vector_x, vector_y);

		Point2f vertical_cross_point = getVerticalCrossPointOfLinePoint(&v4line, pCenter);

		Point2f point; 
		translateAndRotatePoint(vertical_cross_point, point, fTranslate_x, fTranslate_y, fAngle, pCenter);

		float middle_line_normal = sqrt(vector_x * vector_x + vector_y * vector_y);

		vector_x /= middle_line_normal;
		vector_y /= middle_line_normal;
		Vec4f rotateLine = Vec4f(vector_x, vector_y, point.x, point.y);
		v4Rst=rotateLine;
	}
	catch (const std::exception& ex) {
		v4Rst = Vec4f();
	}
}

void CvAffineTransForm::rotateAndTranslateLine(cv::Vec4f v4line, cv::Vec4f & v4Rst, float fAngle, cv::Point2f pCenter, float fTranslate_x, float fTranslate_y)
{
	try {
		float vector_x, vector_y;
		rotateVector(v4line[0], v4line[1], fAngle, vector_x, vector_y);

		Point2f vertical_cross_point = getVerticalCrossPointOfLinePoint(&v4line, pCenter);

		Point2f point;
		rotateAndTranslatePoint(vertical_cross_point, point,  fAngle, pCenter, fTranslate_x, fTranslate_y);

		float middle_line_normal = sqrt(vector_x * vector_x + vector_y * vector_y);

		vector_x /= middle_line_normal;
		vector_y /= middle_line_normal;
		Vec4f rotateLine = Vec4f(vector_x, vector_y, point.x, point.y);
		v4Rst= rotateLine;
	}
	catch (const std::exception& ex) {
		v4Rst=Vec4f();
	}
}

void CvAffineTransForm::mySobelWithDirect(cv::Mat img, cv::Mat& Rst, bool isX, bool isReverse)
{
	Mat kern;
	if (isX) {
		if (!isReverse) {
			kern = (Mat_<char>(3, 3) << -1, 0, 1,
				-2, 0, 2,
				-1, 0, 1);
		}
		else {
			kern = (Mat_<char>(3, 3) << 1, 0, -1,
				2, 0, -2,
				1, 0, -1);
		}
	}
	else {
		if (!isReverse) {
			kern = (Mat_<char>(3, 3) << -1, -2, -1,
				0, 0, 0,
				1, 2, 1);
		}
		else {
			kern = (Mat_<char>(3, 3) << 1, 2, 1,
				0, 0, 0,
				-1, -2, -1);
		}
	}

	filter2D(img, Rst, img.depth(), kern);

}

void CvAffineTransForm::normalizeLineDir(cv::Vec4f& line)
{
	try
	{
		double vx = line[0];
		double vy = line[1];
		double sqrtvxy = std::sqrt(vx * vx + vy * vy);
		if (std::abs(sqrtvxy) < 0.001) return;
		line[0] /= sqrtvxy;
		line[1] /= sqrtvxy;
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CvAffineTransForm::correctLineDir(cv::Vec4f& line)
{
	try
	{
		if (line[0] == 0)
		{
			line[1] = std::abs(line[1]);
		}
		else
		{
			if (line[0] < 0)
			{
				line[0] = -line[0];
				line[1] = -line[1];
			}
		}
		
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CvAffineTransForm::correctLineDirByX(cv::Vec4f& line, bool isXPositive)
{
	try
	{
		if (line[0] != 0)
		{
			if (isXPositive)
			{
				if (line[0] < 0)
				{
					line[0] = -line[0];
					line[1] = -line[1];
				}
			}
			else
			{
				if (line[0] > 0)
				{
					line[0] = -line[0];
					line[1] = -line[1];
				}
			}
		}

	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CvAffineTransForm::correctLineDirByY(cv::Vec4f& line, bool isYPositive)
{
	try
	{
		if (line[1] != 0)
		{
			if (isYPositive)
			{
				if (line[1] < 0)
				{
					line[0] = -line[0];
					line[1] = -line[1];
				}
			}
			else
			{
				if (line[1] > 0)
				{
					line[0] = -line[0];
					line[1] = -line[1];
				}
			}
		}

	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CvAffineTransForm::getLineAngle(double& angle, cv::Vec4f& line, bool isAngle)
{
	try
	{
		double vx = line[0];
		double vy = line[1];
		if (std::abs(vx) < 0.0001)
		{
			if (vy > 0) angle = CV_PI / 2.0;
			else angle = -CV_PI / 2.0;
		}
		else
		{
			angle = std::atan2(vy, vx);
		}

		if (isAngle)
		{
			angle = angle * 180 / CV_PI;
		}
	}
	catch (std::exception& ex)
	{
		// todo
	}
}

void CvAffineTransForm::getLineAngle1(double& angle, cv::Vec4f& line, bool isAngle)
{
	getLineAngle(angle, line, isAngle);
	angle = -angle;
}


