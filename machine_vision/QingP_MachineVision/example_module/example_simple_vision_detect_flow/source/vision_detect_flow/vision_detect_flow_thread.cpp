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

	qDebug() << QStringLiteral("1.�򵥵�ȡ�񣨶�ȡ����ͼƬ��");
	CameraCommonStruct::CameraCaptureFrame stCaptureImage;
	m_pclsCameraSingleFileOpenCV->setCaptureImagePathname(strImagePathname);
	m_pclsCameraSingleFileOpenCV->getCaptureImage(stCaptureImage);
	
	qDebug() << QStringLiteral("2.�򵥵�ͼ��Ԥ��������ԭ����ͼ��");
	cvmTestImage = m_pclsCombineIdentity->transform(stCaptureImage.cvmImage);

	qDebug() << QStringLiteral("3.�򵥵�ͼ����ʾ��ʹ��imshow����ʾ5s��");
	showImage(cvmTestImage);

	qDebug() << QStringLiteral("4.�򵥵�vision��ⷽʽ����ͼ���ϻ�һ��Բ�Σ�");
	cvmResultImage = detectImage(cvmTestImage);

	qDebug() << QStringLiteral("5.�򵥵ļ��������ʾ��ʹ��imshow����ʾ5s)");
	showImage(cvmResultImage);

	qDebug() << QStringLiteral("6.�򵥵ı���ԭͼ");
	m_pclsSaveOneImage->m_stParam.cvmImage = cvmTestImage;
	m_pclsSaveOneImage->m_stParam.strImagePathname = "D:/TestImage_01.bmp";
	SaveOneImage::SaveOneImageResult stSaveTestImageResult;
	m_pclsSaveOneImage->saveImage(stSaveTestImageResult);

	qDebug() << QStringLiteral("7.�򵥵ı�����ͼ");
	m_pclsSaveOneImage->m_stParam.cvmImage = cvmResultImage;
	m_pclsSaveOneImage->m_stParam.strImagePathname = "D:/ResultImage_01.bmp";
	SaveOneImage::SaveOneImageResult stSaveResultImageResult;
	m_pclsSaveOneImage->saveImage(stSaveResultImageResult);

	qDebug() << QStringLiteral("8.�򵥵ļ���ԭͼ");
	m_pclsLoadOneImage->m_stParam.strImagePathname = "D:/ResultImage_01.bmp";
	LoadOneImage::LoadOneImageResult stLoadTestImageResult;
	m_pclsLoadOneImage->loadImage(stLoadTestImageResult);

	qDebug() << QStringLiteral("9.����ʾ���ص�ԭͼ��ʹ��imshow����ʾ5s��");
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
