#include "serializer.h"
#include <cstdint>
#include <iostream>
#include <iterator>
#include <opencv2/core/hal/interface.h>

using cv::Mat;
using std::vector;

vector<uint8_t> Serializer::u16ToBytes(uint16_t val)
{
    vector<uint8_t> vec(2);

    vec[0] = val;
    vec[1] = val >> 8;

    return vec;
}

uint16_t Serializer::u16FromBytes(const std::vector<uint8_t> & bytes)
{
    return bytes[0] | bytes[1] << 8;
}

std::vector<uint8_t> Serializer::u32ToBytes(uint32_t val)
{
    vector<uint8_t> vec(4);

    vec[0] = val;
    vec[1] = val >> 8;
    vec[2] = val >> 16;
    vec[3] = val >> 24;


    return vec;
}

uint32_t Serializer::u32FromBytes(const std::vector<uint8_t> & bytes)
{
    return bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24;
}

uint32_t Serializer::getNumberOfBytes(const cv::Mat & image)
{
    const auto imageHeaderSize = sizeof(image.cols)+sizeof(image.rows)+sizeof(image.type());
    const auto imageDataSize = image.total() * image.elemSize();

    return imageHeaderSize + imageDataSize;
}

vector<uint8_t> Serializer::MatToBytes(const cv::Mat & image)
{
    vector<uint8_t> bytes;
    bytes.reserve(getNumberOfBytes(image));

    const auto rows = u32ToBytes(image.rows);
    bytes.insert(bytes.end(),rows.begin(),rows.end());

    const auto cols = u32ToBytes(image.cols);
    bytes.insert(bytes.end(),cols.begin(),cols.end());

    const auto type = u32ToBytes(image.type());
    bytes.insert(bytes.end(), type.begin(),type.end());

    for (int i = 0; i < image.rows; ++i) {
        bytes.insert(bytes.end(),image.ptr<uchar>(i),image.ptr<uchar>(i)+image.cols*image.channels());
    }

    return bytes;
}

cv::Mat Serializer::MatFromBytes(std::vector<uint8_t> & bytes)
{
   const auto rows = u32FromBytes(bytes);
   bytes.erase(bytes.begin(),bytes.begin() + sizeof(uint32_t));
   
   const auto cols = u32FromBytes(bytes);
   bytes.erase(bytes.begin(),bytes.begin() + sizeof(uint32_t));

   const auto type = u32FromBytes(bytes);
   bytes.erase(bytes.begin(),bytes.begin() + sizeof(uint32_t));

   auto image = Mat(rows,cols,type, reinterpret_cast<void*>(bytes.data()));
  
   return image;
}