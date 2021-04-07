#include <iostream>
#include<opencv2/opencv.hpp>

#define KRATA_PATH "krata.png"
#define DRUK_PATH "druk.png"

void testCanny(int poz, void* data)
{
	cv::Mat* tabImg = (cv::Mat*)data;
	int c1 = cv::getTrackbarPos("Canny 1", "test");
	int c2 = cv::getTrackbarPos("Canny 2", "test");
	cv::Canny(tabImg[0], tabImg[1], c1, c2);
	cv::imshow("test", tabImg[1]);
}

void progowanie(int poz, void* data)
{
	cv::Mat* tabImg = (cv::Mat*)data;
	cv::threshold(tabImg[0], tabImg[1], poz, 255, cv::THRESH_TRIANGLE);
	//cv::THRESH_BINARY, cv::THRESH_BINARY_INV, cv::THRESH_TRUNC, cv::THRESH_TOZERO, 
	//cv::THRESH_TOZERO_INV, cv::THRESH_OTSU, cv::THRESH_TRIANGLE
	cv::imshow("test", tabImg[1]);
}

void progowanieAdaptacyjne(int poz, void* data)
{
	cv::Mat* tabImg = (cv::Mat*)data;
	cv::adaptiveThreshold(tabImg[0], tabImg[1], 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 49, poz - 128);
	//cv::ADAPTIVE_THRESH_MEAN_C, cv::ADAPTIVE_THRESH_GAUSSIAN_C)
	cv::imshow("test", tabImg[1]);
}

int main()
{
	int zn = 'k';
	cv::Mat imgKrata = cv::imread(KRATA_PATH, 0);
	cv::Mat imgDruk = cv::imread(DRUK_PATH, 0);
	cv::Mat workImg;
	cv::Mat tmp1, tmp2;
	do
	{
		switch (zn)
		{
		case 'k':
			workImg = imgKrata.clone();
			break;
		case 'd':
			workImg = imgDruk.clone();
			break;
		case 'b':
			cv::blur(workImg, workImg, cv::Size(5, 5));
			break;
		case 'g':
			cv::GaussianBlur(workImg, workImg, cv::Size(5, 15), 0);
			break;
		case 's':
			cv::Sobel(~workImg, workImg, -1, 1, 0, 3);
			break;
		case 'S':
			cv::Sobel(workImg, workImg, -1, 1, 0, 3);
			break;
		case 'G':
			cv::Sobel(workImg, tmp1, -1, 0, 1, 3);
			cv::Sobel(workImg, tmp2, -1, 1, 0, 3);
			tmp1 |= tmp2;
			workImg = ~workImg;
			cv::Sobel(workImg, tmp2, -1, 1, 0, 3);
			tmp1 |= tmp2;
			cv::Sobel(workImg, tmp2, -1, 0, 1, 3);
			tmp1 |= tmp2;
			workImg = tmp1;
			break;
		case 'r':
			cv::Sobel(workImg, tmp1, CV_16S, 1, 0, 3);
			cv::Sobel(workImg, tmp2, CV_16S, 0, 1, 3);
			tmp1 = cv::abs(tmp1) + cv::abs(tmp2);
			tmp1.convertTo(workImg, CV_8UC1);
			break;
		case 'f':
		{
			cv::Mat_<float> kernel(5, 5);
			kernel << 1, 1, 0, -1, -1
					, 1, 1, 0, -1, -1
					, 0, 0, 0, 0, 0
					, -1, -1, 0, 1, 1
					, -1, -1, 0, 1, 1;
			cv::filter2D(workImg, tmp1, -1, kernel);
			cv::filter2D(workImg, tmp2, -1, -kernel);
			workImg = tmp1 | tmp2;
		}
		break;
		case 'c':
		{
			cv::Mat tabImg[2] = { workImg, workImg.clone() };
			cv::createTrackbar("Canny 1", "test", NULL, 255, testCanny, tabImg);
			cv::createTrackbar("Canny 2", "test", NULL, 255, testCanny, tabImg);
			cv::waitKey();
			cv::destroyWindow("test");
			workImg = tabImg[1];
		}
		break;
		case 't':
		{

			cv::Mat tabImg[2] = { workImg, workImg.clone() };
			cv::createTrackbar("prog", "test", NULL, 255, progowanie, tabImg);
			cv::waitKey();
			cv::destroyWindow("test");
			workImg = tabImg[1];
		}
		break;
		case 'a':
		{

			cv::Mat tabImg[2] = { workImg, workImg.clone() };
			cv::createTrackbar("A prog", "test", NULL, 255, progowanieAdaptacyjne, tabImg);
			cv::waitKey();
			cv::destroyWindow("test");
			workImg = tabImg[1];
		}
		break;
		case 'x':
		{
			//1).Zaimplementować pod przyciskiem ‘x’ funkcję, która wyświetli wynik operacji :
			//obraz - blur(obraz, 9x9)
			cv::blur(workImg, workImg, cv::Size(9, 9));
		}
		break;
		case 'y':
		{
			//2) Zaimplementować pod przyciskiem ‘y’ funkcję, która wyświetli wynik operacji :
			//GaussianBlur(obraz, 3x3) - GaussianBlur(obraz, 7x7)
			cv::GaussianBlur(workImg, workImg, cv::Size(3, 3), 0);
			cv::GaussianBlur(workImg, workImg, cv::Size(7, 7), 0);

		}
		break;
		}
		cv::imshow("test", workImg);
	} while ((zn = cv::waitKey()) != 'q');
}