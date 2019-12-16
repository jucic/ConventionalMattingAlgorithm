#include "stdafx.h"
#include "globalmatting.h"

// you can get the guided filter implementation
// from https://github.com/atilimcetin/guided-filter
#include "guidedfilter.h"

using namespace std;
using namespace cv;

void get_Trimap(Mat& trimap);

int main()
{
	cv::Mat image = cv::imread("./data/_BGR.bmp", 1);
	cv::Mat trimap = cv::imread("./data/_DL.bmp", 0);

	//cv::resize(image, image, cv::Size(1000, 750));
	get_Trimap(trimap);

	cv::imwrite("./data/trimap.png", trimap);
	cv::imwrite("./data/image.png", image);

	const auto T1 = std::chrono::system_clock::now();

	expansionOfKnownRegions(image, trimap, 9);
	cv::Mat foreground, alpha;
	globalMatting(image, trimap, foreground, alpha);
	// filter the result with fast guided filter
	//alpha = guidedFilter(image, alpha, 10, 1e-5);

	const auto T2 = std::chrono::system_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(T2 - T1).count();
	std::cout << "GlobalMatting time: " << duration << "[ms]" << std::endl << std::endl;


	for (int x = 0; x < trimap.cols; ++x)
		for (int y = 0; y < trimap.rows; ++y)
		{
			if (trimap.at<uchar>(y, x) == 0)
				alpha.at<uchar>(y, x) = 0;
			else if (trimap.at<uchar>(y, x) == 255)
				alpha.at<uchar>(y, x) = 255;
		}

	cv::imwrite("./data/res.png", alpha);


	system("pause");
	return 0;
}

void get_Trimap(Mat& trimap) {

	for (int i = 0; i < trimap.rows; i++)
	{
		for (int j = 0; j < trimap.cols; j++)
		{
			trimap.at<unsigned char>(i, j) = 255 - trimap.at<unsigned char>(i, j);
		}
	}

	cv::Mat trimap_dilate(trimap.rows, trimap.cols, CV_8UC1);
	cv::Mat element_d = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10));
	cv::dilate(trimap, trimap_dilate, element_d);//≈Ú’Õ
	cv::imwrite("./data/trimap_dilate.bmp", trimap_dilate);

	cv::Mat trimap_erode(trimap.rows, trimap.cols, CV_8UC1);
	cv::Mat element_e = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10));
	cv::erode(trimap, trimap_erode, element_e);//∏Ø ¥
	cv::imwrite("./data/trimap_erode.bmp", trimap_erode);

	for (int i = 0; i < trimap.rows; i++)
	{
		for (int j = 0; j < trimap.cols; j++)
		{
			if (trimap_erode.at<unsigned char>(i, j) == 255)
				trimap.at<unsigned char>(i, j) = 255;
			else if(trimap_dilate.at<unsigned char>(i, j) == 0)
				trimap.at<unsigned char>(i, j) = 0;
			else
				trimap.at<unsigned char>(i, j) = 128;
		}
	}

	//for (int i = 0; i < trimap.rows; i++)
	//{
	//	for (int j = 0; j < trimap.cols; j++)
	//	{
	//		if (trimap_erode.at<unsigned char>(i, j) > 0 && trimap_erode.at<unsigned char>(i, j) < 127)
	//		{
	//			trimap_erode.at<unsigned char>(i, j) = 0;
	//		}
	//		if (trimap_dilate.at<unsigned char>(i, j) > 0 && trimap_dilate.at<unsigned char>(i, j) < 127)
	//		{
	//			trimap_dilate.at<unsigned char>(i, j) = 255;
	//		}
	//	}
	//}

	//for (int i = 0; i < trimap.rows; i++)
	//{
	//	for (int j = 0; j < trimap.cols; j++)
	//	{
	//		if (trimap_erode.at<unsigned char>(i, j) != 255 && trimap_dilate.at<unsigned char>(i, j) == 255)
	//			trimap.at<unsigned char>(i, j) = 128;
	//	}
	//}

	// (optional) exploit the affinity of neighboring pixels to reduce the 
	// size of the unknown region. please refer to the paper
	// 'Shared Sampling for Real-Time Alpha Matting'.

	cv::resize(trimap, trimap, cv::Size(1000*4, 750 * 4));

	cout << trimap.elemSize1() << endl;

	//for(int i=0;i<trimap.rows;i++)
	//	for (int j = 0; j < trimap.cols; j++) {
	//		if (trimap.at<uchar>(i, j) > 0 && trimap.at<uchar>(i, j) < 255)
	//			trimap.at<uchar>(i, j) = 128;
	//	}

	for (int i = 0; i < trimap.rows; i++) {
		uchar* data = trimap.ptr<uchar>(i);
		for (int j = 0; j < trimap.cols; j++) {
			if (data[j] > 0 && data[j] < 255)
				data[j] = 128;
		}
	}
}