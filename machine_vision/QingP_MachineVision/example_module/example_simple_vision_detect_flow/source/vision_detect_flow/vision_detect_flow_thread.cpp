#include "vision_detect_flow_thread.h"

VisionDetectFlowThread::VisionDetectFlowThread()
{
	m_pclsCameraSingleFileOpenCV = new CameraSingleFileOpenCV(CameraCommonEnum::CameraCaptureType::SingleFileOpenCV, CameraCommonEnum::CameraFrameType::OpenCVMatType);
	m_pclsCombineIdentity = new CombineIdentity();
	m_pclsSaveOneImage = new SaveOneImage();
	m_pclsLoadOneImage = new LoadOneImage();
}

VisionDetectFlowThread::~VisionDetectFlowThread()
{
	delete m_pclsCameraSingleFileOpenCV;
	delete m_pclsCombineIdentity;
	delete m_pclsSaveOneImage;
	delete m_pclsLoadOneImage;
}

void VisionDetectFlowThread::run()
{
	detect();
}

void VisionDetectFlowThread::detect()
{
	qDebug() << "Test Vision Detect Flow : run ...";

	const QString& strImagePathname = "D:/lena.bmp";
	cv::Mat cvmTestImage;
	cv::Mat cvmResultImage;

	qDebug() << QStringLiteral("1.简单的取像（读取本地图片）");
	CameraCommonStruct::CameraCaptureFrame stCaptureImage;
	m_pclsCameraSingleFileOpenCV->setCaptureImagePathname(strImagePathname);
	m_pclsCameraSingleFileOpenCV->getCaptureImage(stCaptureImage);
	
	qDebug() << QStringLiteral("2.简单的图像预处理（返回原本的图像）");
	cvmTestImage = m_pclsCombineIdentity->transform(stCaptureImage.cvmImage);

	qDebug() << QStringLiteral("3.简单的图像显示（使用imshow，显示5s）");
	showImage(cvmTestImage);

	qDebug() << QStringLiteral("4.简单的vision检测方式（在图像上画一个圆形）");
	cvmResultImage = detectImage(cvmTestImage);

	qDebug() << QStringLiteral("5.简单的检测结果再显示（使用imshow，显示5s)");
	showImage(cvmResultImage);

	qDebug() << QStringLiteral("6.简单的保存原图");
	m_pclsSaveOneImage->m_stParam.cvmImage = cvmTestImage;
	m_pclsSaveOneImage->m_stParam.strImagePathname = "D:/TestImage_01.bmp";
	SaveOneImage::SaveOneImageResult stSaveTestImageResult;
	m_pclsSaveOneImage->saveImage(stSaveTestImageResult);

	qDebug() << QStringLiteral("7.简单的保存结果图");
	m_pclsSaveOneImage->m_stParam.cvmImage = cvmResultImage;
	m_pclsSaveOneImage->m_stParam.strImagePathname = "D:/ResultImage_01.bmp";
	SaveOneImage::SaveOneImageResult stSaveResultImageResult;
	m_pclsSaveOneImage->saveImage(stSaveResultImageResult);

	qDebug() << QStringLiteral("8.简单的加载原图");
	m_pclsLoadOneImage->m_stParam.strImagePathname = "D:/ResultImage_01.bmp";
	LoadOneImage::LoadOneImageResult stLoadTestImageResult;
	m_pclsLoadOneImage->loadImage(stLoadTestImageResult);

	qDebug() << QStringLiteral("9.简单显示加载的原图（使用imshow，显示5s）");
	showImage(stLoadTestImageResult.cvmImage);
}

void VisionDetectFlowThread::showImage(cv::Mat cvmImage)
{
	if (cvmImage.empty())
	{
		qDebug() << "showImage :: image is empty";
		return;
	}
	cv::imshow("ShowImageWindow", cvmImage);
	cv::waitKey();
}

cv::Mat VisionDetectFlowThread::detectImage(cv::Mat cvmImage)
{
	cv::Mat cvmResultImage = cvmImage.clone();
	int iImageWidth = cvmResultImage.cols;
	int iImageHeight = cvmResultImage.rows;
	cv::Point cvpiPaintCenter(iImageWidth / 2, iImageHeight / 2);
	int iCircleRadius = (iImageWidth / 3 < iImageHeight / 3 ? iImageWidth / 3 : iImageHeight / 3);
	cv::circle(cvmResultImage, cvpiPaintCenter, iCircleRadius, \
		cv::Scalar(0, 0, 255), 5);
	return cvmResultImage;
}
