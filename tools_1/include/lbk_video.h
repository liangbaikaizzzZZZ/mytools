#pragma once
#include <iostream>
#include"lbk_opencv_tools.h"

//NV12转YUV420
void NV12_to_I420(uint8_t* nv12data, uint8_t* i420data, int frameWidth, int frameHeight);
//NV12转YUV444
void NV12_to_I444(uint8_t* nv12data, uint8_t* i444data, int frameWidth, int frameHeight);
//P010转I420
void P010le_to_I420(uint8_t* p010data, uint8_t* i420data, int frameWidth, int frameHeight);
void P010_to_I420(uint8_t* P010data, uint8_t* I420data, int frameWidth, int frameHeight);
