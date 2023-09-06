#include "RansacAlgorithm.h"
#include "linesegmentdetector.h"

using namespace cv;
using namespace std;

std::vector<cv::Point2d> RansacAlgorithm::deleteRepeatedContinuousPoints(vector<Point2d> _points1, vector<Point2d> _points2, bool isX /*= true*/)
{
	vector<Point2d> result;

	int index_i = 0;
	int index_j = 0;

	if (_points1.size()<_points2.size())
	{
		swap(_points1, _points2);
	}

	while ((index_i < _points1.size()) && (index_j < _points2.size()))
	{
		if (isX)
		{
			if (_points1.at(index_i).x == _points2.at(index_j).x)
			{
				index_j++;
				index_i++;			
			}
			else
			{
				result.push_back(_points1.at(index_i));
				index_i++;

			}

		}
		else
		{
			if (_points1.at(index_i).y < _points2.at(index_j).y)
			{
				index_i++;
				index_j++;
			
			}
			else
			{
				result.push_back(_points1.at(index_i));
				index_i++;
				
			}

		}
	}
	return result;

}

void RansacAlgorithm::RansacFilter(const vector<Point2d>& points, vector<Point2d> &result_points, vector<Point2d> &discard_points, float threshold, int N, int _min, int _max)
{
	result_points.clear();
	discard_points.clear();
	if (points.size() < 6)
	{
		result_points = points;
		return;
		//return points;
	}
	int max_dismatch_count = points.size() * _min / 100;
	int min_match_count = points.size() * _max / 100;
	//vector<Point2d>  result_points;
	Point2d onePoint, twoPoint;
	int maxSize = 0;
	try
	{
		for (int i = 0; i < points.size() - N; i++)
		{
			int max_dismatch_count_temp = 0;
			int min_match_count_temp = 0;
			vector<Point2d>  resultPoints;//符合直线的点个数
			vector<Point2d> discardPoints;//不符合直线的点个数
			resultPoints.reserve(points.size());
			discardPoints.reserve(points.size());
			onePoint = points.at(i);
			twoPoint = points.at(i + N);
			//Vec4f point_line(onePoint.x, onePoint.y, twoPoint.x, twoPoint.y);
			double A = onePoint.y - twoPoint.y;
			double B = -(onePoint.x - twoPoint.x);
			double C = onePoint.x * twoPoint.y - onePoint.y * twoPoint.x;//Ax+By+C=0
			double denominator = sqrt(A * A + B * B);

			for (int j = 0; j < points.size(); j++)
			{
				if (/*CLineSegmentDetector::DisOfPointToLine(point_line, points.at(j))*/abs(A * points[j].x + B * points[j].y + C) / denominator <= threshold)
				{
					resultPoints.push_back(points.at(j));
					min_match_count_temp++;
					if (min_match_count_temp > min_match_count)
					{
						break;
					}
				}
				else
				{
					discardPoints.push_back(points.at(j));
					max_dismatch_count_temp++;
					if (max_dismatch_count_temp > max_dismatch_count)
					{
						break;
					}
				}
			}
			if (min_match_count_temp > min_match_count)
			{
				result_points = resultPoints;
				return;
				//return resultPoints;
			}
			if (resultPoints.size() > maxSize)
			{
				maxSize = resultPoints.size();
				result_points.clear();
				result_points = resultPoints;
				discard_points.clear();
				discard_points = discardPoints;
			}
		}
	}
	catch (std::exception& ex)
	{
		// todo
	}
	return;
	//return result_points;
}

void RansacAlgorithm::RansacFilter(const vector<Point>& points, vector<Point> &result_points, vector<Point> &discard_points, float threshold, int N, int _min /*= 100*/, int _max /*= 100*/)
{
	result_points.clear();
	discard_points.clear();
	if (points.size() < 6)
	{
		result_points = points;
		return;
		//return points;
	}
	int max_dismatch_count = points.size() * _min / 100;
	int min_match_count = points.size() * _max / 100;
	//vector<Point>  result_points;
	Point2d onePoint, twoPoint;
	int maxSize = 0;
	try
	{
		for (int i = 0; i < points.size() - N; i++)
		{
			int max_dismatch_count_temp = 0;
			int min_match_count_temp = 0;
			vector<Point>  resultPoints;//符合直线的点个数
			vector<Point>  discardPoints;//不符合直线的点个数
			resultPoints.reserve(points.size());
			discardPoints.reserve(points.size());
			onePoint = points.at(i);
			twoPoint = points.at(i + N);
			//Vec4f point_line(onePoint.x, onePoint.y, twoPoint.x, twoPoint.y);
			double A = onePoint.y - twoPoint.y;
			double B = -(onePoint.x - twoPoint.x);
			double C = onePoint.x * twoPoint.y - onePoint.y * twoPoint.x;//Ax+By+C=0
			double denominator = sqrt(A * A + B * B);
			for (int j = 0; j < points.size(); j++)
			{
				if (/*CLineSegmentDetector::DisOfPointToLine(point_line, points.at(j))*/abs(A * points[j].x + B * points[j].y + C) / denominator <= threshold)
				{
					resultPoints.push_back(points.at(j));
					min_match_count_temp++;
					if (min_match_count_temp > min_match_count)
					{
						break;
					}
				}
				else
				{
					discardPoints.push_back(points.at(j));
					max_dismatch_count_temp++;
					if (max_dismatch_count_temp > max_dismatch_count)
					{
						break;
					}
				}
			}
			if (min_match_count_temp > min_match_count)
			{
				result_points = resultPoints;
				return;
				//return resultPoints;
			}
			if (resultPoints.size() > maxSize)
			{
				maxSize = resultPoints.size();
				result_points.clear();
				result_points = resultPoints;
				discard_points.clear();
				discard_points = discardPoints;
			}
		}
	}
	catch (std::exception& ex)
	{
		// todo
	}
	return;
	//return result_points;
}

vector<cv::Point2d> RansacAlgorithm::RansacFilterHorVer(vector<Point2d> points, int threshold, int N, bool isHor, int angleScope)
{
	if (points.size() < 6)
	{
		return points;
	}

	vector<vector<Point2d>> resultLines1;
	vector<Point2d> result_points;
	Point2d onePoint, twoPoint;
	Vec4f line_Dir;
	double rad = angleScope*CV_PI / 180;

	try
	{
		for (int i = 0; i < points.size() - N; i++)
		{
			vector<Point2d>  resultPoints;//符合直线的点个数
			onePoint = points.at(i);
			twoPoint = points.at(i + N);
			Vec4f point_line(onePoint.x, onePoint.y, twoPoint.x, twoPoint.y);
			for (int j = 0; j < points.size(); j++)
			{
				if (CLineSegmentDetector::DisOfPointToLine(point_line, points.at(j)) <= threshold)
				{
					resultPoints.push_back(points.at(j));
				}
			}
			resultLines1.push_back(resultPoints);
		}



		//	for (int i = 0; i < resultLines1.size() - 1; i++)
		//	{
		//		for (int j = i + 1; j < resultLines1.size(); j++)
		//		{
		//			if (resultLines1.at(i).size() < resultLines1.at(j).size())
		//			{
		//// 				vector<Point2d> temp_points = resultLines1.at(i);
		//// 				resultLines1.at(i) = resultLines1.at(j);
		//// 				resultLines1.at(j) = temp_points;
		//				swap(resultLines1.at(i), resultLines1.at(j));
		//			}
		//		}
		//	}

		sort(resultLines1.begin(), resultLines1.end(), [](vector<Point2d>& a, vector<Point2d>& b)->bool {
			return a.size() > b.size();
			});

		for (int i = 0; i < resultLines1.size(); i++)
		{
			if (resultLines1.at(i).size() > 5)
			{
				fitLine(resultLines1.at(i), line_Dir, cv::DIST_L2, 0, 0.01, 0.01);
				if (isHor)
				{
					if (abs(line_Dir[1]) < sin(rad))
					{
						result_points = resultLines1.at(i);
						break;
					}
				}
				else
				{
					if (abs(line_Dir[1]) > cos(rad))
					{
						result_points = resultLines1.at(i);
						break;
					}
				}
			}
			else
			{
				result_points = resultLines1.at(i);
			}
		}
	}
	catch (std::exception& ex)
	{
		// todo
	}

	// 	Point2d onePoint, twoPoint;
	// 	int maxSize = 0;
	// 	double rad = angleScope*cv::PI / 180;
	// 	vector<Point2d> result_points;
	// 	for (int i = 0; i < points.size() - 1 - N; i++)
	// 	{
	// 		vector<Point2d>  resultPoints;//符合直线的点个数
	// 		onePoint = points.at(i);
	// 		for (int j = i + 1; j < i + 1 + N; j++)
	// 		{
	// 			twoPoint = points.at(j);
	// 			Vec4f point_line(onePoint.x, onePoint.y, twoPoint.x, twoPoint.y);
	// 			double tempValue = abs(onePoint.y - twoPoint.y) / sqrt((onePoint.x - twoPoint.x)*(onePoint.x - twoPoint.x) + (onePoint.y - twoPoint.y)*(onePoint.y - twoPoint.y));
	// 			if (isHor)
	// 			{
	// 				if (tempValue < sin(rad))
	// 				{
	// 					break;
	// 				}
	// 			}
	// 			else
	// 			{
	// 				if (tempValue > cos(rad))
	// 				{
	// 					break;
	// 				}
	// 			}
	// 			if (CLineSegmentDetector::DisOfPointToLine(point_line, points.at(j)) <= threshold)
	// 			{
	// 				resultPoints.push_back(points.at(j));
	// 			}
	// 		}
	// 		if (resultPoints.size() > maxSize)
	// 		{
	// 			maxSize = resultPoints.size();
	// 			result_points.clear();
	// 			result_points = resultPoints;
	// 		}
	// 	}

	if (result_points.size() < 6)
	{
		return points;
	}
	else
	{
		return result_points;
	}
}

std::vector<cv::Point2d> RansacAlgorithm::RansacFilterHorVerMin(vector<Point2d> points, int threshold, int N, bool isHor, Vec4f &_line, int angleScope)
{
	if (points.size() < 6)
	{
		return points;
	}

	vector<vector<Point2d>> resultLines1;
	vector<Point2d> result_points;
	Point2d onePoint, twoPoint;
	Vec4f line_Dir;
	double rad = angleScope*CV_PI / 180;

	try
	{
		for (int i = 0; i < points.size() - N; i++)
		{
			vector<Point2d>  resultPoints;//符合直线的点个数
			onePoint = points.at(i);
			twoPoint = points.at(i + N);
			Vec4f point_line(onePoint.x, onePoint.y, twoPoint.x, twoPoint.y);

			// 			double angle = 0;
			// 			if (twoPoint.y == onePoint.y)
			// 			{
			// 				angle = 180;
			// 			}
			// 			else
			// 			{
			// 				angle = atan(abs(onePoint.x - twoPoint.x) / abs(onePoint.y - twoPoint.y)) * 180 / cv::PI;
			// 			}
			// 	 
			// 			if (isHor)
			// 			{
			// 				if (angle>angleScope)
			// 				{
			// 					continue;
			// 				}
			// 			}
			// 			else
			// 			{
			// 				if (abs(angle-90)>angleScope)
			// 				{
			// 					continue;
			// 				}
			// 			}

			for (int j = 0; j < points.size(); j++)
			{
				if (CLineSegmentDetector::DisOfPointToLine(point_line, points.at(j)) <= threshold)
				{
					resultPoints.push_back(points.at(j));
				}
			}
			if (resultPoints.size() > points.size() * 0.5)
				resultLines1.push_back(resultPoints);
		}



		// 	for (int i = 0; i < resultLines1.size() - 1; i++)
		// 	{
		// 		for (int j = i + 1; j < resultLines1.size(); j++)
		// 		{
		// 			if (resultLines1.at(i).size() < resultLines1.at(j).size())
		// 			{
		// 				vector<Point2d> temp_points = resultLines1.at(i);
		// 				resultLines1.at(i) = resultLines1.at(j);
		// 				resultLines1.at(j) = temp_points;
		// 			}
		// 		}
		// 	}


		double min_angle = 361;

		for (int i = 0; i < resultLines1.size(); i++)
		{
			if (resultLines1.at(i).size() > 5)
			{
				fitLine(resultLines1.at(i), line_Dir, cv::DIST_L2, 0, 0.01, 0.01);
				if (isHor)
				{
					if (abs(line_Dir[1]) < sin(rad) && abs(line_Dir[1]) < min_angle)
					{
						result_points = resultLines1.at(i);
						_line = line_Dir;
						min_angle = abs(line_Dir[1]);
					}
				}
				else
				{
					if (abs(line_Dir[1]) > cos(rad) && abs(line_Dir[1]) < min_angle)
					{
						result_points = resultLines1.at(i);
						_line = line_Dir;
						min_angle = abs(line_Dir[1]);
					}
				}
			}
			// 		else
			// 		{
			// 			result_points = resultLines1.at(i);
			// 		}
		}
	}
	catch (std::exception& ex)
	{
		// todo
	}

	// 	Point2d onePoint, twoPoint;
	// 	int maxSize = 0;
	// 	double rad = angleScope*cv::PI / 180;
	// 	vector<Point2d> result_points;
	// 	for (int i = 0; i < points.size() - 1 - N; i++)
	// 	{
	// 		vector<Point2d>  resultPoints;//符合直线的点个数
	// 		onePoint = points.at(i);
	// 		for (int j = i + 1; j < i + 1 + N; j++)
	// 		{
	// 			twoPoint = points.at(j);
	// 			Vec4f point_line(onePoint.x, onePoint.y, twoPoint.x, twoPoint.y);
	// 			double tempValue = abs(onePoint.y - twoPoint.y) / sqrt((onePoint.x - twoPoint.x)*(onePoint.x - twoPoint.x) + (onePoint.y - twoPoint.y)*(onePoint.y - twoPoint.y));
	// 			if (isHor)
	// 			{
	// 				if (tempValue < sin(rad))
	// 				{
	// 					break;
	// 				}
	// 			}
	// 			else
	// 			{
	// 				if (tempValue > cos(rad))
	// 				{
	// 					break;
	// 				}
	// 			}
	// 			if (CLineSegmentDetector::DisOfPointToLine(point_line, points.at(j)) <= threshold)
	// 			{
	// 				resultPoints.push_back(points.at(j));
	// 			}
	// 		}
	// 		if (resultPoints.size() > maxSize)
	// 		{
	// 			maxSize = resultPoints.size();
	// 			result_points.clear();
	// 			result_points = resultPoints;
	// 		}
	// 	}
	
	return result_points;
}
