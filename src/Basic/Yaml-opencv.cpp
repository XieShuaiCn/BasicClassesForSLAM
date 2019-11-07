//
// Created by xieshuai on 19-11-6.
//
#include "Basic/Yaml-opencv.h"

#if defined(SLAM_HAVE_OPENCV) && defined(SLAM_HAVE_YAML)
namespace YAML
{
    // convert 'YAML::Node' to 'template Mat'
    template<typename _Ty>
    cv::Mat ConvertToMatT(const Node &node, int rows, int cols)
    {
        cv::Mat __mat(rows, cols, SLAM::Basic::TypeFromStd<_Ty>::CvCode);
        for (int i = 0; i < rows; ++i) {
            _Ty *p = (_Ty *) (__mat.ptr(i));
            for (int j = 0; j < cols; ++j) {
                *p = node[i * cols + j].as<_Ty>();
                ++p;
            }
        }
        return __mat;
    }

    // convert 'template Mat' to 'YAML::Node'
    template<typename _Ty>
    void ConvertFromMatT(Node &node, const cv::Mat &mat)
    {
        node["rows"] = Node(mat.rows);
        node["cols"] = Node(mat.cols);
        node["dt"] = Node(char(SLAM::Basic::TypeFromStd<_Ty>::Name));
        Node node_data;
        for (int i = 0; i < mat.rows; ++i) {
            _Ty *p = (_Ty *) (mat.ptr(i));
            for (int j = 0; j < mat.cols; ++j) {
                node_data.push_back<_Ty>(*(p + j));
            }
        }
        node["data"] = node_data;
    }

    // Specific template of cv::Mat converter
    Node convert<cv::Mat>::encode(const cv::Mat &m)
    {
        Node node;
        switch (m.depth()) {
            case CV_64F:
                ConvertFromMatT<double>(node, m);
                break;
            case CV_32F:
                ConvertFromMatT<float>(node, m);
                break;
            case CV_32S:
                ConvertFromMatT<int>(node, m);
                break;
            case CV_16S:
                ConvertFromMatT<short>(node, m);
                break;
            case CV_16U:
                ConvertFromMatT<unsigned short>(node, m);
                break;
            case CV_8S:
                ConvertFromMatT<char>(node, m);
                break;
            case CV_8U:
                ConvertFromMatT<unsigned char>(node, m);
                break;
            default:
                ConvertFromMatT<unsigned char>(node, m);
                break;
        }
        return node;
    }

    bool convert<cv::Mat>::decode(const Node &node, cv::Mat &mat)
    {
        if (!node.IsMap() || node.size() != 4) {
            return false;
        }
        // size
        int rows = node["rows"].as<int>();
        int cols = node["cols"].as<int>();
        if (rows == 0 || cols == 0)
            return false;
        // type
        char dt = node["dt"].as<char>();
        auto node_data = node["data"];
        switch (dt) {
            case 'd':
                mat = ConvertToMatT<double>(node_data, rows, cols);
                break;
            case 'f':
                mat = ConvertToMatT<float>(node_data, rows, cols);
                break;
            case 'i':
                mat = ConvertToMatT<int>(node_data, rows, cols);
                break;
            case 's':
                mat = ConvertToMatT<short>(node_data, rows, cols);
                break;
            case 'w':
                mat = ConvertToMatT<unsigned short>(node_data, rows, cols);
                break;
            case 'c':
                mat = ConvertToMatT<char>(node_data, rows, cols);
                break;
            case 'u':
                mat = ConvertToMatT<unsigned char>(node_data, rows, cols);
                break;
            default:
                return false;
        }
        return true;
    }
}
#endif