//
// Created by xieshuai on 19-11-6.
//


#ifndef SLAM_YAML_OPENCV_H
#define SLAM_YAML_OPENCV_H

#include "Yaml.h"
#ifdef SLAM_HAVE_OPENCV

#include <opencv2/core.hpp>

#ifdef SLAM_HAVE_YAML
namespace YAML
{
    // Specific template of cv::Mat converter
    template<>
    struct convert<cv::Mat>
    {
        static Node encode(const cv::Mat &m);

        static bool decode(const Node &node, cv::Mat &mat);
    };

    // output cv::Mat to YAML::Emitter
    Emitter &operator<<(Emitter &out, const cv::Mat &mat);
}
#endif

////////////////////////// implement ////////////////////////
#ifdef SLAM_HAVE_YAML
namespace YAML
{
    // output cv::Mat to YAML::Emitter
    inline Emitter &operator<<(Emitter &out, const cv::Mat &mat)
    {
        out << convert<cv::Mat>::encode(mat);
        return out;
    }
}
#endif

#endif //SLAM_YAML_OPENCV_H

#endif