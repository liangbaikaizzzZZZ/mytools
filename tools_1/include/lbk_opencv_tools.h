#pragma once
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//resize图片 
void mpt_resize(std::string srcdir, std::string dstdir, int width, int height);

//将文件夹下的图片统一重命名
void mpt_rename(std::string srcdir, std::string dstdir);

//中值滤波cuda
void mpt_medianBlur_cuda(cv::Mat& src, cv::Mat& dst);
//中值滤波
void mpt_medianBlur(cv::Mat& src, cv::Mat& dst);

//视频中提取图片
void mpt_extract_pic_from_video(std::string videopath, std::string picpath);

//视频中提取图片并合成九宫格图片
void mpt_extract_merge_pic_from_video(std::string videopath, std::string picpath, int gap = 1);
