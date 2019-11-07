//
// Created by xieshuai on 19-11-6.
//
#include "Basic/Yaml-eigen.h"
#ifdef SLAM_HAVE_EIGEN
#ifdef SLAM_HAVE_YAML
namespace YAML
{
    template
    struct convert<Eigen::Matrix < double, 3, 1>>;
}
#elif defined(SLAM_HAVE_OPENCV)

#endif
#endif