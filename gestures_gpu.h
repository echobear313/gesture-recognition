#ifndef GESTURERECOGNITION_GESTURE_GPU_H
#define GESTURERECOGNITION_GESTURE_GPU_H

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/gpu/gpu.hpp"
#include <assert.h>

void getMask(const gpu::GpuMat& im, gpu::GpuMat& mask);

#endif //GESTURERECOGNITION_GESTURE_GPU_H