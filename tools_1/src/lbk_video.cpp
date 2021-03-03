#include "lbk_video.h"

//NV12 转 I420 
void NV12_to_I420(uint8_t* nv12data, uint8_t* i420data, int frameWidth, int frameHeight) {
	uint8_t* nv12_src[2];
	nv12_src[0] = nv12data;
	nv12_src[1] = nv12_src[0] + frameWidth * frameHeight;

	uint8_t* yuv420_dst[3];
	yuv420_dst[0] = i420data;
	yuv420_dst[1] = yuv420_dst[0] + frameWidth * frameHeight;
	yuv420_dst[2] = yuv420_dst[1] + frameWidth * frameHeight / 4;

	//Y
	memcpy(yuv420_dst[0], nv12_src[0], frameWidth * frameHeight);
	// for(int i = 0; i < frameWidth * frameHeight ; i++){
	//     *(yuv420_dst[0]++) = *(nv12_src[0] + i);
	// }

	for (int i = 0; i < frameWidth * frameHeight / 2; i++) {
		unsigned char b = *(nv12_src[1] + i);
		if (i % 2 == 0) {//U
			*(yuv420_dst[1]++) = *(nv12_src[1] + i);
		}
		else {//V
			*(yuv420_dst[2]++) = *(nv12_src[1] + i);
		}
	}
}

//NV12转YUV444
void NV12_to_I444(uint8_t* nv12data, uint8_t* i444data, int frameWidth, int frameHeight) {
	uint8_t* nv12_src[2];
	nv12_src[0] = nv12data;
	nv12_src[1] = nv12_src[0] + frameWidth * frameHeight;

	uint8_t* yuv444_dst[3];
	yuv444_dst[0] = i444data;
	yuv444_dst[1] = yuv444_dst[0] + frameWidth * frameHeight;
	yuv444_dst[2] = yuv444_dst[1] + frameWidth * frameHeight;

	//Y
	memcpy(yuv444_dst[0], nv12_src[0], frameWidth * frameHeight);

	//UV
	for (int i = 0; i < frameWidth * frameHeight / 2; i++) {
		if (i % 2 == 0) {//U,NV12中一位对应444中4位
			*(yuv444_dst[1]) = *(nv12_src[1] + i);
			*(yuv444_dst[1] + frameWidth) = *(nv12_src[1] + i);
			yuv444_dst[1]++;
			*(yuv444_dst[1]) = *(nv12_src[1] + i);
			*(yuv444_dst[1] + frameWidth) = *(nv12_src[1] + i);
			yuv444_dst[1]++;
		}
		else {//V,NV12中一位对应444中4位
			*(yuv444_dst[2]) = *(nv12_src[1] + i);
			*(yuv444_dst[2] + frameWidth) = *(nv12_src[1] + i);
			yuv444_dst[2]++;
			*(yuv444_dst[2]) = *(nv12_src[1] + i);
			*(yuv444_dst[2] + frameWidth) = *(nv12_src[1] + i);
			yuv444_dst[2]++;
		}
		if ((i > frameWidth && i % frameWidth == 0)) {//UV分量跳行
			yuv444_dst[1] = yuv444_dst[1] + frameWidth;
			yuv444_dst[2] = yuv444_dst[2] + frameWidth;
		}
	}
}

//P010转I420
void P010le_to_I420(uint8_t* p010data, uint8_t* i420data, int frameWidth, int frameHeight) {

	uint8_t* p010_src[2];
	p010_src[0] = p010data;
	p010_src[1] = p010_src[0] + frameWidth * frameHeight * 2;

	uint8_t* yuv420_dst[3];
	yuv420_dst[0] = i420data;
	yuv420_dst[1] = yuv420_dst[0] + frameWidth * frameHeight;
	yuv420_dst[2] = yuv420_dst[1] + frameWidth * frameHeight / 4;
	uint16_t Y, U, V;

	//Y
	for (int i = 0; i < frameWidth * frameHeight; i++) {
		Y = *((uint16_t*)p010_src[0] + i) >> 6;
		Y = Y < 64 ? 64 : Y;
		Y = Y > 940 ? 940 : Y;
		*(yuv420_dst[0]++) = (uint8_t)(Y >> 2);
	}

	//UV
	for (int i = 0; i < frameWidth * frameHeight / 2; i++) {
		if (i % 2 == 0) {
			U = (*((uint16_t*)p010_src[1] + i)) & 0x00ff;
			*(yuv420_dst[1]++) = U;
		}
		else {
			V = (*((uint16_t*)p010_src[1] + i)) & 0x00ff;
			*(yuv420_dst[2]++) = V;
		}
	}
	cv::Mat src, dst2;
	cv::Mat myuv(frameHeight + frameHeight / 2, frameWidth, CV_8UC1, (unsigned char*)i420data);
	cv::cvtColor(myuv, src, cv::COLOR_YUV2BGR_I420);
	cv::namedWindow("src", 0);
	cv::imshow("src", src);
	cv::waitKey();

}

//P010转I420
void P010_to_I420(uint8_t* P010data, uint8_t* I420data, int frameWidth, int frameHeight) {
	uint8_t* p010_src[2];
	p010_src[0] = P010data;
	p010_src[1] = p010_src[0] + frameWidth * frameHeight * 2;

	uint8_t* yuv420_dst[3];
	yuv420_dst[0] = I420data;
	yuv420_dst[1] = yuv420_dst[0] + frameWidth * frameHeight;
	yuv420_dst[2] = yuv420_dst[1] + frameWidth * frameHeight / 4;
	uint8_t Y, U, V;

	//Y
	for (int i = 0; i < frameWidth * frameHeight; i++) {
		Y = *((uint16_t*)p010_src[0] + i) >> 8;
		Y = Y < 16 ? 16 : (Y > 235 ? 235 : Y);
		*(yuv420_dst[0]++) = Y;
	}

	//UV
	for (int i = 0; i < frameWidth * frameHeight / 2; i++) {
		if (i % 2 == 0) {
			U = (*((uint16_t*)p010_src[1] + i)) >> 8;
			U = (((U - 128) * (200 + 256)) >> 8) + 128;
			U = U < 16 ? 16 : (U > 235 ? 235 : U);
			*(yuv420_dst[1]++) = U;
		}
		else {
			V = (*((uint16_t*)p010_src[1] + i)) >> 8;
			V = (((V - 128) * (200 + 256)) >> 8) + 128;
			V = V < 16 ? 16 : (V > 235 ? 235 : V);
			*(yuv420_dst[2]++) = V;
		}
	}

}
