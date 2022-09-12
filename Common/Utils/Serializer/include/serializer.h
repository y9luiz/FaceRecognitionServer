#pragma once

#include <vector>
#include <cstdint>
#include <opencv2/core/mat.hpp>


class Serializer
{
    public:
        static std::vector<uint8_t> u16ToBytes(uint16_t val);
        static uint16_t u16FromBytes(const std::vector<uint8_t> & bytes);
        static std::vector<uint8_t> u32ToBytes(uint32_t val);
        static uint32_t u32FromBytes(const std::vector<uint8_t> & bytes);

        static uint32_t getNumberOfBytes(const cv::Mat & image);
        static std::vector<uint8_t> MatToBytes(const cv::Mat & image);
        static cv::Mat MatFromBytes(std::vector<uint8_t> & bytes);
};