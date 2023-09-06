#ifndef ALGORITHM_OPERATOR_RANSACALGORITHM_H
#define ALGORITHM_OPERATOR_RANSACALGORITHM_H

#include "../../algorithm_operator_global.h"

#include <opencv2/opencv.hpp>

class ALGORITHM_OPERATOR_EXPORT  RansacAlgorithm
{
public:
	template <typename _Tp>
	static std::vector<cv::Point_<_Tp>> RansacFilter(const std::vector<cv::Point_<_Tp>>& points, const float& threshold, const int& step, const int& _min = 100, const int& _max = 100)
	{
		if (points.size() < 6)
		{
			return points;
		}
		std::vector<cv::Point_<_Tp>> result_points(points.size());
		std::vector<cv::Point_<_Tp>> resultPoints(points.size());//符合直线的点个数

		int max_dismatch_count = points.size() * _min / 100;
		int min_match_count = points.size() * _max / 100;
		int maxSize = 0;

		for (int i = 0, j = step; j < points.size(); i++, j++)
		{
			resultPoints.clear();
			int max_dismatch_count_temp = 0;
			const auto &onePoint = points[i];
			const auto &twoPoint = points[j];

			double A = onePoint.y - twoPoint.y;
			double B = twoPoint.x - onePoint.x;
			if (A == 0 && B == 0) { continue; }
			double C = onePoint.x * twoPoint.y - onePoint.y * twoPoint.x;//Ax+By+C=0
			double denominator = sqrt(A * A + B * B) * threshold;

			for (auto &point : points)
			{
				if (abs(A * point.x + B * point.y + C) <= denominator)
				{
					resultPoints.emplace_back(point);
					if (resultPoints.size() > min_match_count) { break; }
				}
				else
				{
					++max_dismatch_count_temp;
					if (max_dismatch_count_temp > max_dismatch_count) { break; }
				}
			}
			if (resultPoints.size() > min_match_count)
			{
				return resultPoints;
			}
			if (resultPoints.size() > maxSize)
			{
				maxSize = resultPoints.size();
				result_points = resultPoints;
			}
		}

		return result_points;
	}

	static void RansacFilter(const std::vector<cv::Point2d>& _points, std::vector<cv::Point2d> &result_points, std::vector<cv::Point2d> &discard_points, float _threshold, int _step, int _min = 100, int _max = 100);
	static void RansacFilter(const std::vector<cv::Point>& _points, std::vector<cv::Point> &result_points, std::vector<cv::Point> &discard_points, float _threshold, int _step, int _min = 100, int _max = 100);

	static std::vector<cv::Point2d> deleteRepeatedContinuousPoints(std::vector<cv::Point2d> _points1, std::vector<cv::Point2d> _points2, bool isX = true);
	static std::vector<cv::Point2d> RansacFilterHorVer(std::vector<cv::Point2d> _points, int _threshold, int _step, bool isHor, int angleScope = 30);
	static std::vector<cv::Point2d> RansacFilterHorVerMin(std::vector<cv::Point2d> points, int threshold, int N, bool isHor, cv::Vec4f &_line, int angleScope = 30);

private:
	RansacAlgorithm() = delete;
	~RansacAlgorithm() = delete;
};

#endif 