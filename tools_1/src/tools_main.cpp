#include <io.h>
#include <windows.h>

#include "lbk_opencv_tools.h"
#include "lbk_video.h"
#include "lbk_basetool.h"



#if 0 //测试GPU中值滤波
int main() {
	cv::Mat src = cv::imread("./test.jpg", 1);
	if (src.empty()) {
		return 0;
	}

	cv::Mat dst;
	int i = 100;
	while (i--) {
		//mpt_medianBlur_cuda(src, dst);
		mpt_medianBlur(src, dst);
	}

	cv::namedWindow("src", 0);
	cv::namedWindow("dst", 0);
	cv::imshow("src", src);
	cv::imshow("dst", dst);
	cv::waitKey(0);
}
#endif

#if 0 //图片尺寸预处理
int main() {
	//std::string srcdir = "C:\\Users\\langd\\mytest\\四分类\\DataSets\\train\\monitor";
	//std::string dstdir = "C:\\Users\\langd\\mytest\\四分类\\data\\train\\monitor";
	//mpt_resize(srcdir, dstdir, 227, 227);
}
#endif

#if 0 //视频格式转换
int main() {
	int frameWidth = 1376;
	int frameHeight = 768;
	int frameSize = frameWidth * frameHeight * 3 / 2;

	uint8_t* bufferSrc = (uint8_t*)malloc(frameSize * 2);
	uint8_t* bufferDst = (uint8_t*)malloc(frameSize);
	FILE* file, * out;
	fopen_s(&file, "./video/P10_1376_768.yuv", "rb");
	fopen_s(&out, "./video/out_1376_768_I420.yuv", "ab+");
	if (!file || !out) {
		std::cout << "File open failed.";
	}
	else {
		if (fseek(file, 0, 0) == 0) {
			while (1) {
				memset(bufferSrc, 0, frameSize * 2);
				memset(bufferDst, 0, frameSize);
				size_t size = fread(bufferSrc, frameSize * 2, 1, file);
				std::cout << "size:" << size << "  " << frameSize << std::endl;
				if (size != 1) {
					break;
				}
				P010le_to_I420(bufferSrc, bufferDst, frameWidth, frameHeight);
				fwrite(bufferDst, 1, frameSize, out);
			}

			fclose(file);
			fclose(out);

		}
		else {
			std::cout << "File seek failed.";
		}
	}

	free(bufferSrc);
	free(bufferDst);
}
#endif


#if 0 //批量图片重命名
int main() {
	mpt_rename("Z:/photos/图片打标带间隙people_gap/素材/", "Z:/photos/图片打标带间隙people_gap/20201209/");
}
#endif

#if 0 //视频中提取图片
int main() {
	mpt_extract_pic_from_video("./tmp/test.mp4", "./tmp/");
}
#endif

#if 1 //视频中提取图片并合成9宫格图片
int main() {
	mpt_extract_merge_pic_from_video("./tmp/test.mp4", "./tmp/", 10);
}
#endif
