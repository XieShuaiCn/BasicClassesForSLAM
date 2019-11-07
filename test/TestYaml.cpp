//
// Created by xieshuai on 19-11-7.
//
#include "Basic/Yaml.h"
#include "Basic/Yaml-eigen.h"
#include "Basic/Yaml-opencv.h"
#include <gtest/gtest.h>
#ifdef SLAM_HAVE_EIGEN
#include <Eigen/Core>
#endif
#ifdef SLAM_HAVE_OPENCV
#include <opencv2/core.hpp>
#endif

#ifdef SLAM_HAVE_EIGEN
TEST(Yaml, WriteEigen)
{
    Eigen::Matrix3i mat1;
    mat1 << 1, 2, 3, 4, 5, 6, 7, 8, 9;
    SLAM::Basic::YamlWriter writer("../test/mat.yaml");
    writer.Write("m", mat1);
}
TEST(Yaml, ReadEigen)
{
    Eigen::Matrix3i mat2;
    SLAM::Basic::YamlReader reader("../test/mat.yaml");
    ASSERT_TRUE(reader.Read("m", mat2));
    ASSERT_EQ(mat2(0,0), 1);
    ASSERT_EQ(mat2(0,1), 2);
    ASSERT_EQ(mat2(0,2), 3);
    ASSERT_EQ(mat2(1,0), 4);
    ASSERT_EQ(mat2(1,1), 5);
    ASSERT_EQ(mat2(1,2), 6);
    ASSERT_EQ(mat2(2,0), 7);
    ASSERT_EQ(mat2(2,1), 8);
    ASSERT_EQ(mat2(2,2), 9);
}
#endif

#ifdef SLAM_HAVE_OPENCV
TEST(Yaml, WriteCvMat)
{
    cv::Mat mat1 = (cv::Mat_<int>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);
    SLAM::Basic::YamlWriter writer("../test/mat.yaml");
    writer.Write("m", mat1);
}

TEST(Yaml, ReadCvMat)
{
    cv::Mat mat2;
    SLAM::Basic::YamlReader reader("../test/mat.yaml");
    ASSERT_TRUE(reader.Read("m", mat2));
    ASSERT_EQ(mat2.at<int>(0, 0), 1);
    ASSERT_EQ(mat2.at<int>(0,1), 2);
    ASSERT_EQ(mat2.at<int>(0,2), 3);
    ASSERT_EQ(mat2.at<int>(1,0), 4);
    ASSERT_EQ(mat2.at<int>(1,1), 5);
    ASSERT_EQ(mat2.at<int>(1,2), 6);
    ASSERT_EQ(mat2.at<int>(2,0), 7);
    ASSERT_EQ(mat2.at<int>(2,1), 8);
    ASSERT_EQ(mat2.at<int>(2,2), 9);
}
#endif
