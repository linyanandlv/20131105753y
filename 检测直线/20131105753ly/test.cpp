#include <cv.h>
#include <highgui.h>
#include <math.h>
int main(int argc, char** argv)
{
	const char* filename = argc >= 2 ? argv[1] : "D:/5.jpg";//获取图片
	IplImage* src = cvLoadImage( filename, 0 );
	IplImage* dst;
	IplImage* color_dst;
	CvMemStorage* storage = cvCreateMemStorage(0);//内存采用默认大小
	CvSeq* lines = 0;
	int i;
	if( !src )
		return -1;//显示原图像
	dst = cvCreateImage( cvGetSize(src), 8, 1 );
	color_dst = cvCreateImage( cvGetSize(src), 8, 3 );
	cvCanny( src, dst, 50, 200, 3 );
	cvCvtColor( dst, color_dst, CV_GRAY2BGR );//显示灰度图像

#if 0
	lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 100, 0, 0 );
	for( i = 0; i < MIN(lines->total,100); i++ )
	{
		float* line = (float*)cvGetSeqElem(lines,i);
		float rho = line[0];
		float theta = line[1];
		CvPoint pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 5, CV_AA, 0 );

    }

#else
	lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 50, 10 );
	for( i = 0; i < lines->total; i++ )//如果检测出的直线超过0条，只画出0条
	{
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
		cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 5, CV_AA, 0 );//画出检测的直线

    }

#endif
	cvNamedWindow( "Source", 1 );
	cvShowImage( "Source", src );
	cvNamedWindow( "Hough", 1 );
	cvShowImage( "Hough", color_dst );
	cvWaitKey(0);
	return 0;

}
