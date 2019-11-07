//
// Created by xieshuai on 19-11-6.
//

#ifndef SLAM_YAML_EIGEN_H
#define SLAM_YAML_EIGEN_H

#include "Yaml.h"
#ifdef SLAM_HAVE_EIGEN
#include <Eigen/Core>
#ifdef SLAM_HAVE_OPENCV

#include <opencv2/core/eigen.hpp>
#endif

#ifdef SLAM_HAVE_YAML
namespace YAML
{
    // Specific template of Eigen::Matrix template
    template<typename _T, int _R, int _C>
    struct convert<Eigen::Matrix<_T, _R, _C>>
    {
        static Node encode(const Eigen::Matrix<_T, _R, _C> &m);

        static bool decode(const Node &node, Eigen::Matrix<_T, _R, _C> &m);
    };

    // output Eigen::Matrix to YAML::Emitter
    template<typename _T, int _R, int _C>
    Emitter &operator<<(Emitter &out, const Eigen::Matrix<_T, _R, _C> &mat);

    extern template struct convert<Eigen::Matrix<double, 3, 1>>;
}
#elif defined(SLAM_HAVE_OPENCV)
namespace cv{

    template<typename _Scalar, int _Rows, int _Cols>
    struct NodeConverter<Eigen::Matrix<_Scalar, _Rows, _Cols>>{
        static bool read(const FileNode &node,
                         Eigen::Matrix<_Scalar, _Rows, _Cols> &value,
                         const Eigen::Matrix<_Scalar, _Rows, _Cols> &default_value);
        static bool write(FileStorage &file, const String& name,
                          const Eigen::Matrix<_Scalar, _Rows, _Cols> &value);
    };
}
#endif

//////////////////////////// implements //////////////////////////

#ifdef SLAM_HAVE_YAML
namespace YAML
{
    // Specific template of Eigen::Matrix template
    template<typename _T, int _R, int _C>
    Node convert<Eigen::Matrix<_T, _R, _C>>::encode(const Eigen::Matrix<_T, _R, _C> &m)
    {
        Node node, node_data;
        node_data.SetStyle(YAML::EmitterStyle::Flow);
        node["rows"] = Node(m.rows());
        node["cols"] = Node(m.cols());
        node["dt"] = char(
                SLAM::Basic::TypeFromStd<typename Eigen::internal::traits<Eigen::Matrix<_T, _R, _C>>::Scalar>::Name);
        for (int i = 0; i < _R; ++i) {
            for (int j = 0; j < _C; ++j) {
                node_data.push_back(m(i, j));
            }
        }
        node["data"] = node_data;
        return node;
    }

    template<typename _T, int _R, int _C>
    bool convert<Eigen::Matrix<_T, _R, _C>>::decode(const Node &node, Eigen::Matrix<_T, _R, _C> &m)
    {
        if (node.IsMap())
        {
            // size
            int rows = SLAM::Basic::YamlReader::ReadNamedNodeSafe<int>(node, "rows", _R);
            int cols = SLAM::Basic::YamlReader::ReadNamedNodeSafe<int>(node, "cols", _C);
            if (rows != _R || cols != _C)
                return false;
            // type
            std::string dt = SLAM::Basic::YamlReader::ReadNamedNodeSafe<std::string>(node, "dt", "d");
            if (dt.size() != 1 || dt[0] != SLAM::Basic::TypeFromStd<_T>::Name)
                return false;
            auto node_data = node["data"];
            for (int i = 0; i < _R; ++i) {
                for (int j = 0; j < _C; ++j) {
                    m(i, j) = node_data[i * _C + j].as<_T>();
                }
            }
            return true;
        }
        else if(node.IsSequence())
        {
            if(node.size() != _R *_C)
                return false;
            for (int i = 0; i < _R; ++i) {
                for (int j = 0; j < _C; ++j) {
                    m(i, j) = node[i * _C + j].as<_T>();
                }
            }
            return true;
        }
        return false;
    }

    // output Eigen::Matrix to YAML::Emitter
    template<typename _T, int _R, int _C>
    inline Emitter &operator<<(Emitter &out, const Eigen::Matrix<_T, _R, _C> &mat)
    {
        out << convert<Eigen::Matrix<_T, _R, _C>>::encode(mat);
        return out;
    }
}
#elif defined(SLAM_HAVE_OPENCV)
namespace cv
{

    template<typename _Scalar, int _Rows, int _Cols>
    bool NodeConverter<Eigen::Matrix<_Scalar, _Rows, _Cols>>::read(const FileNode &node,
                Eigen::Matrix<_Scalar, _Rows, _Cols> &value,
                const Eigen::Matrix<_Scalar, _Rows, _Cols> &default_value){
        cv::Mat mat, def;
        cv::eigen2cv(default_value, def);
        cv::read(node, mat, def);
        if (mat.empty()) return false;
        if (mat.rows != _Rows) return false;
        if (mat.cols != _Cols) return false;
        //if(SLAM::Basic::TypeFromStd<_Scalar>::CvCode != mat.depth())
        //    return false;
        cv::cv2eigen(mat, value);
        return true;
    }
    template<typename _Scalar, int _Rows, int _Cols>
    bool NodeConverter<Eigen::Matrix<_Scalar, _Rows, _Cols>>::write(FileStorage &file, const String& name,
                const Eigen::Matrix<_Scalar, _Rows, _Cols> &value) {
        cv::Mat mat;
        cv::eigen2cv(value, mat);
        cv::write(file, name, mat);
        return true;
    }
}
#endif

#endif

#endif //SLAM_YAML_EIGEN_H