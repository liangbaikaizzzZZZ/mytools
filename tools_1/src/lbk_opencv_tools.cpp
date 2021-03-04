#include"lbk_opencv_tools.h"
#include <io.h>
#include <vector>
#include <windows.h>
#include <iostream>

//获取文件夹下的图片
void mpt_getfile_v(std::vector<std::string>& v, std::string srcdir) {
	std::string serchdir = srcdir;
	if (*(srcdir.end() - 1) != '\\') {
		serchdir += "\\*.jpg";
	}

	struct _finddata_t m_fileInfo;
	long long handle = 0;
	handle = _findfirst(serchdir.c_str(), &m_fileInfo);
	if (0 >= handle)
	{
		printf("请检查图片路径...\n");
		assert(0 < hFile);
	}
	do {
		if (!(m_fileInfo.attrib & _A_SUBDIR))
		{
			//std::cout << m_fileInfo.name << std::endl;
			v.push_back(m_fileInfo.name);
		}
	} while (_findnext(handle, &m_fileInfo) == 0);
}

//将图片resize
void mpt_resize(std::string srcdir, std::string dstdir, int width, int height) {
	std::vector<std::string> pic_v;
	mpt_getfile_v(pic_v, srcdir);

	for (int i = 0; i < pic_v.size(); i++) {
		if (*(srcdir.end() - 1) != '\\') {
			srcdir += "\\";
		}
		if (*(dstdir.end() - 1) != '\\') {
			dstdir += "\\";
		}

		std::string pic_full_path = srcdir + pic_v[i];
		std::cout << pic_full_path << std::endl;
		std::string dst_pic_full_path = dstdir + pic_v[i];

		cv::Mat src = cv::imread(pic_full_path, 1);
		assert(src.empty());
		cv::Mat dst;
		cv::resize(src, dst, cv::Size(width, height), 0, 0, 0);
		cv::imwrite(dst_pic_full_path, dst);
	}
}

//将图片rename
void mpt_rename(std::string srcdir, std::string dstdir)
{
	std::vector<std::string> pic_v;
	mpt_getfile_v(pic_v, srcdir);

	for (int i = 0; i < pic_v.size(); i++) {
		std::string pic_full_path = srcdir + pic_v[i];
		std::string dst_pic_full_path = dstdir + "20201209" + std::to_string(i) + ".jpg";
		cv::Mat src = cv::imread(pic_full_path, 1);
		assert(src.empty());
		std::cout << pic_full_path << std::endl;
		cv::imwrite(dst_pic_full_path, src);
	}
}

//中值滤波cpu版本
void mpt_medianBlur(cv::Mat& src, cv::Mat& dst) {
	double t1 = GetTickCount64();
	cv::medianBlur(src, dst, 7);
	double t2 = GetTickCount64();
	std::cout << "time :" << t2 - t1 << std::endl;
}

/*视频中提取图片
@param videopath 视频地址
@param picpath   提取的图片存放地址，末尾加上斜杠
*/
void mpt_extract_pic_from_video(std::string videopath, std::string picpath)
{
	cv::VideoCapture capture;
	capture.open(videopath);// 打开文件
	if (!capture.isOpened()) {
		printf("read video file failed !\n");
		return;
	}
	cv::Mat frame;
	int i = 0, j = 0;
	while (capture.read(frame)) {
		std::string pic = "";
		pic = picpath + "20201210004" + std::to_string(j) + ".jpg";
		if (i % 300 == 0) {
			cv::imwrite(pic, frame);
			j++;
		}
		i++;
	}
	capture.release();
}

/*视频中提取图片并合成九宫格图片
@param videopath 视频地址
@param picpath   九宫格图片存放地址，末尾加上斜杠
@param gap		 视频中间隔gap个帧选一个
*/
void mpt_extract_merge_pic_from_video(std::string videopath, std::string picpath, int gap)
{
	cv::VideoCapture capture;
	capture.open(videopath);// 打开文件
	if (!capture.isOpened()) {
		printf("read video file failed !\n");
		return;
	}
	cv::Mat frame, dst;
	std::vector<cv::Mat> nine_pic_v;
	int i = 0, j = 0;
	while (capture.read(frame)) {
		if (i % gap == 0) {
			cv::Mat tmp_m = frame.clone();
			nine_pic_v.push_back(tmp_m);
			if (nine_pic_v.size() == 9) {
				//merge
				cv::Mat vmat1, vmat2, vmat3;
				cv::Mat hmatArray[] = { nine_pic_v[0],nine_pic_v[1],nine_pic_v[2] };//水平方向三个图片
				cv::hconcat(hmatArray, 3, vmat1);//水平反向三个图片合成
				hmatArray[0] = nine_pic_v[3];
				hmatArray[1] = nine_pic_v[4];
				hmatArray[2] = nine_pic_v[5];
				cv::hconcat(hmatArray, 3, vmat2);
				hmatArray[0] = nine_pic_v[6];
				hmatArray[1] = nine_pic_v[7];
				hmatArray[2] = nine_pic_v[8];
				cv::hconcat(hmatArray, 3, vmat3);
				//将水平方向合成的图片再垂直合并到一起
				cv::Mat vmatArray[] = { vmat1 ,vmat2 ,vmat3 };
				cv::vconcat(vmatArray, 3, dst);
				nine_pic_v.clear();
				//save
				j++;
				std::string pic = "";
				pic = picpath + "20201210004" + std::to_string(j) + ".jpg";
				cv::imwrite(pic, dst);
			}
		}
		i++;
	}
	capture.release();
}

//中值滤波gpu版本
void mpt_medianBlur_cuda(cv::Mat& src, cv::Mat& dst) {
	double t1 = GetTickCount64();

	//cv::Mat blank_ch = cv::Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC1);//零值通道
	std::vector<cv::Mat> bgrChannels(3);
	split(src, bgrChannels);//分为三个通道

	const cv::cuda::GpuMat src_B(bgrChannels[0]);
	const cv::cuda::GpuMat src_G(bgrChannels[1]);
	const cv::cuda::GpuMat src_R(bgrChannels[2]);
	cv::cuda::GpuMat dst_B, dst_G, dst_R;
	cv::Ptr<cv::cuda::Filter> median = cv::cuda::createMedianFilter(CV_8UC1, 7);
	median->apply(src_B, dst_B);
	median->apply(src_G, dst_G);
	median->apply(src_R, dst_R);

	std::vector<cv::cuda::GpuMat> channels_BGR;
	channels_BGR.push_back(dst_B);
	channels_BGR.push_back(dst_G);
	channels_BGR.push_back(dst_R);
	cv::cuda::GpuMat dst_gpu;
	cv::cuda::merge(channels_BGR, dst_gpu);

	dst_gpu.download(dst);

	double t2 = GetTickCount64();
	std::cout << "time :" << t2 - t1 << std::endl;
}
