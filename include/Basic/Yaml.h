//
// Created by xieshuai on 19-5-24.
//

#ifndef SLAM_YAML_H
#define SLAM_YAML_H

#include "SlamConfig.h"
#include "Basic/Exception.h"
#ifdef SLAM_HAVE_EIGEN
#include <Eigen/Core>
#endif
#if defined(SLAM_HAVE_OPENCV)
#include <opencv2/core.hpp>
#endif
#if defined(SLAM_HAVE_YAML)
#include <yaml-cpp/yaml.h>
#elif !defined(SLAM_HAVE_OPENCV)
#error "You should configure at least one of 'OpenCV' and 'Yaml-cpp'"
#endif

// Switch about if raise a exception when the wanted node is empty.
#define YAML_RAISE_EXCEPTION_IF_RETURN_NULL_NODE

namespace SLAM
{
    namespace Basic
    {
        class YamlReader
        {
        public:
            YamlReader() = default;

#ifdef SLAM_HAVE_YAML
            explicit YamlReader(const YAML::Node &node);
#else
            explicit YamlReader(const cv::FileStorage &root);
            explicit YamlReader(const cv::FileNode &node);
#endif

            explicit YamlReader(const std::string &yaml);

            void LoadFile(const std::string &file);

            void LoadString(const std::string &content);

            bool IsLoaded();

            bool Has(const std::string &name);

            template<typename T>
            T Read(const std::string &name) const;

            template<typename T>
            bool Read(const std::string &name, T &val) const;

            template<typename T>
            T ReadSafe(const std::string &name, const T &default_value) const;

            template<typename T>
            bool ReadSafe(const std::string &name, T &val, const T &default_value) const;

            const YamlReader operator[] (int i) const;

            const YamlReader operator[] (const std::string &name) const;

#ifdef SLAM_HAVE_YAML
            const YAML::Node GetNode(int i) const;

            const YAML::Node GetNode(const std::string &name) const;

            static const YAML::Node GetNode(const YAML::Node &node, const std::string &name);
            template<typename T>
            static T ReadNode(const YAML::Node &node);

            template<typename T>
            static bool ReadNode(const YAML::Node &node, T &val);

            template<typename T>
            static T ReadNodeSafe(const YAML::Node &node, const T &default_value);

            template<typename T>
            static bool ReadNodeSafe(const YAML::Node &node, T &val, const T &default_value);

            template<typename T>
            static T ReadNamedNode(const YAML::Node &node, const std::string &name);

            template<typename T>
            static bool ReadNamedNode(const YAML::Node &node, const std::string &name, T &val);

            template<typename T>
            static T ReadNamedNodeSafe(const YAML::Node &node, const std::string &name, const T &default_value);

            template<typename T>
            static bool ReadNamedNodeSafe(const YAML::Node &node, const std::string &name, T &val,
                                          const T &default_value);
#else
            const cv::FileNode GetNode(int i) const;

            const cv::FileNode GetNode(const std::string &name) const;

            static const cv::FileNode GetNode(const cv::FileNode &node, const std::string &name);
            template<typename T>
            static T ReadNode(const cv::FileNode &node);

            template<typename T>
            static bool ReadNode(const cv::FileNode &node, T &val);

            template<typename T>
            static T ReadNodeSafe(const cv::FileNode &node, const T &default_value);

            template<typename T>
            static bool ReadNodeSafe(const cv::FileNode &node, T &val, const T &default_value);

            template<typename T>
            static T ReadNamedNode(const cv::FileNode &node, const std::string &name);

            template<typename T>
            static bool ReadNamedNode(const cv::FileNode &node, const std::string &name, T &val);

            template<typename T>
            static T ReadNamedNodeSafe(const cv::FileNode &node, const std::string &name, const T &default_value);

            template<typename T>
            static bool ReadNamedNodeSafe(const cv::FileNode &node, const std::string &name, T &val,
                                          const T &default_value);
#endif

        protected:
#ifdef SLAM_HAVE_YAML
            YAML::Node m_root;
#else
            cv::FileStorage m_file;
            cv::FileNode m_root;
#endif
        };

        class YamlWriter
        {
        public:
            YamlWriter() = default;

            explicit YamlWriter(const std::string &file);

            explicit YamlWriter(std::ostream &ostream);

            ~YamlWriter();

            std::string GetString();

            template<typename T>
            void Write(const std::string &name, const T &value);
#ifdef SLAM_HAVE_YAML
            template<typename T>
            static void SaveNode(YAML::Emitter &yaml, const std::string &name, const T &value);
#else
            template<typename T>
            static void SaveNode(cv::FileStorage &yaml, const std::string &name, const T &value);
#endif

        protected:
#ifdef SLAM_HAVE_YAML
            YAML::Emitter m_out;
            std::string m_strFile;
#else
            cv::FileStorage m_out;
#endif
        };

        class YamlIO : public YamlReader, public YamlWriter
        {
        public:
            YamlIO() = default;

            explicit YamlIO(const std::string &file);

            explicit YamlIO(std::iostream &stream);
        };
    }
}

#ifdef SLAM_HAVE_YAML
namespace YAML
{
    /***********Have implemented by yaml-cpp*********************
    // Specific template of std::vector converter........Have implemented by yaml-cpp
    template<typename _Tp>
    struct convert<std::vector<_Tp>>
    {
        static Node encode(const std::vector<_Tp> &vec);

        static bool decode(const Node &node, std::vector<_Tp> &vec);
    };
    // output std::vector to YAML::Emitter,,,,,,,Have implemented by yaml-cpp
    template <typename _Tp>
    Emitter &operator<<(Emitter &out, const std::vector<_Tp> &vec);
    */

    // output std::initializer_list to YAML::Emitter
    template<typename _T>
    Emitter &operator<<(Emitter &out, const std::initializer_list<_T> &lst);

#ifdef SLAM_HAVE_OPENCV
    // Specific template of cv::Mat converter
    template<>
    struct convert<cv::Mat>
    {
        static Node encode(const cv::Mat &m);

        static bool decode(const Node &node, cv::Mat &mat);
    };

    // output cv::Mat to YAML::Emitter
    Emitter &operator<<(Emitter &out, const cv::Mat &mat);
#endif
#ifdef SLAM_HAVE_EIGEN
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
#endif
}
#else //SLAM_HAVE_OPENCV
namespace cv{
    bool readFileNode_bool(const FileNode& node, bool& value, bool default_value);
    bool readFileNode_long(const FileNode& node, long& value, long default_value);
    bool readFileNode_ulong(const FileNode& node, unsigned long& value, unsigned long default_value);

    template <typename _Tp>
    inline bool readFileNode(const FileNode &node, _Tp &value, _Tp default_value)
    {
        if(node.empty() || node.isNone())
            return false;
        cv::read(node, value, default_value);
        return true;
    }

    template <>
    inline bool readFileNode<bool>(const FileNode &node, bool &value, bool default_value)
    {
        return readFileNode_bool(node, value, default_value);
    }
    template <>
    inline bool readFileNode<long>(const FileNode& node, long& value, long default_value)
    {
        return readFileNode_long(node, value, default_value);
    }
    template <>
    inline bool readFileNode<unsigned long>(const FileNode& node, unsigned long& value, unsigned long default_value)
    {
        return readFileNode_ulong(node, value, default_value);
    }
}
#endif // SLAM_HAVE_YAML
//////////////////////////////// implements ////////////////////////////////

namespace SLAM
{
    namespace Basic
    {
        // std Type converter
        template<typename _ty>
        struct TypeFromStd
        {
            using Type = _ty;
            enum
            {
                CvCode = 7,
                Name = (int) 't'
            };
        };

        // cv Type converter
        template<int _tycode>
        struct TypeFromCv
        {
            using Type = unsigned char;
            enum
            {
                CvCode = 7,
                Name = (int) 't'
            };
        };

        template<>
        struct TypeFromStd<unsigned char>
        {
            enum
            {
                CvCode = 0,
                Name = (int) 'u'
            };
        };

        template<>
        struct TypeFromStd<char>
        {
            enum
            {
                CvCode = 1,
                Name = (int) 'c'
            };
        };

        template<>
        struct TypeFromStd<unsigned short>
        {
            enum
            {
                CvCode = 2,
                Name = (int) 'w'
            };
        };

        template<>
        struct TypeFromStd<short>
        {
            enum
            {
                CvCode = 3,
                Name = (int) 's'
            };
        };

        template<>
        struct TypeFromStd<int>
        {
            enum
            {
                CvCode = 4,
                Name = (int) 'i'
            };
        };

        template<>
        struct TypeFromStd<float>
        {
            enum
            {
                CvCode = 5,
                Name = (int) 'f'
            };
        };

        template<>
        struct TypeFromStd<double>
        {
            enum
            {
                CvCode = 6,
                Name = (int) 'd'
            };
        };

        template<>
        struct TypeFromCv<0>
        {
            using Type = unsigned char;
            enum
            {
                CvCode = 0,
                Name = (int) 'u'
            };
        };

        template<>
        struct TypeFromCv<1>
        {
            using Type = char;
            enum
            {
                CvCode = 1,
                Name = (int) 'c'
            };
        };

        template<>
        struct TypeFromCv<2>
        {
            using Type = unsigned short;
            enum
            {
                CvCode = 2,
                Name = (int) 'w'
            };
        };

        template<>
        struct TypeFromCv<3>
        {
            using Type = short;
            enum
            {
                CvCode = 3,
                Name = (int) 's'
            };
        };;

        template<>
        struct TypeFromCv<4>
        {
            using Type = int;
            enum
            {
                CvCode = 4,
                Name = (int) 'i'
            };
        };;

        template<>
        struct TypeFromCv<5>
        {
            using Type = float;
            enum
            {
                CvCode = 5,
                Name = (int) 'f'
            };
        };;

        template<>
        struct TypeFromCv<6>
        {
            using Type = double;
            enum
            {
                CvCode = 6,
                Name = (int) 'd'
            };
        };
    }
}
#ifdef SLAM_HAVE_YAML
namespace YAML
{
#ifdef SLAM_HAVE_OPENCV
    // output cv::Mat to YAML::Emitter
    inline Emitter &operator<<(Emitter &out, const cv::Mat &mat)
    {
        out << convert<cv::Mat>::encode(mat);
        return out;
    }
#endif
#ifdef SLAM_HAVE_EIGEN
    // Specific template of Eigen::Matrix template
    template<typename _T, int _R, int _C>
    Node convert<Eigen::Matrix<_T, _R, _C>>::encode(const Eigen::Matrix<_T, _R, _C> &m)
    {
        Node node, node_data;
        node["rows"] = Node(m.rows());
        node["cols"] = Node(m.cols());
        node["dt"] = char(
                SLAM::Basic::TypeFromStd<typename Eigen::internal::traits<Eigen::Matrix<_T, _R, _C>>::Scalar>::Name);
        for (int i = 0; i < _R * _C; ++i)
        {
            node_data.push_back(m(i));
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
            for (int i = 0; i < _R * _C; ++i)
            {
                m(i) = node_data[i].as<double>();
            }
            return true;
        }
        else if(node.IsSequence())
        {
            if(node.size() != _R *_C)
                return false;
            for (int i = 0; i < _R * _C; ++i)
            {
                m(i) = node[i].as<_T>();
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
#endif
    // output std::initializer_list to YAML::Emitter
    template<typename _T>
    inline Emitter &operator<<(Emitter &out, const std::initializer_list<_T> &lst)
    {
        return EmitSeq(out, lst);
    }
}
#else
namespace cv{

}
#endif
namespace SLAM
{
    namespace Basic
    {

        inline bool YamlReader::IsLoaded()
        {
#ifdef SLAM_HAVE_YAML
            return m_root.IsDefined();
#else
            return m_file.isOpened();
#endif
        }

        inline bool YamlReader::Has(const std::string &name)
        {
#ifdef SLAM_HAVE_YAML
            return !m_root.IsScalar() && m_root[name].IsDefined();
#else
            return m_root.isMap() && !m_root[name].empty();
#endif
        }

        template<typename T>
        inline T YamlReader::Read(const std::string &name) const
        {
            return ReadNamedNode<T>(m_root, name);
        }

        template<typename T>
        inline bool YamlReader::Read(const std::string &name, T &val) const
        {
            return ReadNamedNode<T>(m_root, name, val);
        }

        template<typename T>
        inline T YamlReader::ReadSafe(const std::string &name, const T &default_value) const
        {
            return ReadNamedNodeSafe<T>(m_root, name, default_value);
        }

        template<typename T>
        inline bool YamlReader::ReadSafe(const std::string &name, T &val, const T &default_value) const
        {
            return ReadNamedNodeSafe<T>(m_root, name, val, default_value);
        }

        inline const YamlReader YamlReader::operator[](int i) const
        {
            return YamlReader(m_root[i]);
        }

        inline const YamlReader YamlReader::operator[](const std::string &name) const
        {
            return YamlReader(GetNode(name));
        }

#ifdef SLAM_HAVE_YAML
        inline const YAML::Node YamlReader::GetNode(int i) const
        {
            return m_root[i];
        }

        template<typename T>
        inline T YamlReader::ReadNamedNode(const YAML::Node &node, const std::string &name)
        {
            YAML::Node parameter = (name.empty() ? node : GetNode(node, name));
            if(!node.IsDefined() || node.IsNull())
            {
#ifdef YAML_RAISE_EXCEPTION_IF_RETURN_NULL_NODE
                throw Exception(Exception::ERROR_YAML, "Yaml node has not this key.");
#else
                return T();
#endif
            }
            return node.as<T>();
        }

        template<typename T>
        inline T YamlReader::ReadNamedNodeSafe(const YAML::Node &node, const std::string &name, const T &default_value)
        {
            YAML::Node parameter = (name.empty() ? node : GetNode(node, name));
            return YamlReader::ReadNodeSafe<T>(parameter, default_value);
        }

        template<typename T>
        inline bool YamlReader::ReadNamedNode(const YAML::Node &node, const std::string &name, T &val)
        {
            YAML::Node parameter = (name.empty() ? node : GetNode(node, name));
            return YamlReader::ReadNode<T>(parameter, val);
        }

        template<typename T>
        inline bool YamlReader::ReadNamedNodeSafe(const YAML::Node &node, const std::string &name, T &val,
                                                  const T &default_value)
        {
            YAML::Node parameter = (name.empty() ? node : GetNode(node, name));
            return YamlReader::ReadNodeSafe(parameter, val, default_value);
        }

        template<typename T>
        inline T YamlReader::ReadNode(const YAML::Node &node)
        {
            if(!node.IsDefined() || node.IsNull())
            {
#ifdef YAML_RAISE_EXCEPTION_IF_RETURN_NULL_NODE
                throw Exception(Exception::ERROR_YAML, "Yaml node is null.");
#else
                return T();
#endif
            }
            return node.as<T>();
        }

        template<typename T>
        inline bool YamlReader::ReadNode(const YAML::Node &node, T &val)
        {
            if(node.IsDefined() && !node.IsNull())
            {
                val = node.as<T>();
                return true;
            }
            return false;
        }

        template<typename T>
        inline T YamlReader::ReadNodeSafe(const YAML::Node &node, const T &default_value)
        {
            return (node.IsDefined() && !node.IsNull()) ? node.as<T>() : default_value;
        }

        template<typename T>
        inline bool YamlReader::ReadNodeSafe(const YAML::Node &node, T &val, const T &default_value)
        {
            if(node.IsDefined() && !node.IsNull())
            {
                val = node.as<T>();
                return true;
            }
            else
            {
                val = default_value;
                return false;
            }
        }
#else
        inline const cv::FileNode YamlReader::GetNode(int i) const
        {
            return m_root[i];
        }

        template<typename T>
        inline T YamlReader::ReadNamedNode(const cv::FileNode &node, const std::string &name)
        {
            cv::FileNode parameter = (name.empty() ? node : GetNode(node, name));
            if(parameter.empty() || parameter.isNone())
            {
#ifdef YAML_RAISE_EXCEPTION_IF_RETURN_NULL_NODE
                throw Exception(Exception::ERROR_YAML, "Yaml node has not this key.");
#else
                return T();
#endif
            }
            return ReadNodeSafe(parameter, T());
        }

        template<typename T>
        inline T YamlReader::ReadNamedNodeSafe(const cv::FileNode &node, const std::string &name, const T &default_value)
        {
            auto parameter = (name.empty() ? node : GetNode(node, name));
            return ReadNodeSafe(parameter, default_value);
        }

        template<typename T>
        inline bool YamlReader::ReadNamedNode(const cv::FileNode &node, const std::string &name, T &val)
        {
            auto parameter = (name.empty() ? node : GetNode(node, name));
            return YamlReader::ReadNode<T>(parameter, val);
        }

        template<typename T>
        inline bool YamlReader::ReadNamedNodeSafe(const cv::FileNode &node, const std::string &name, T &val,
                                                  const T &default_value)
        {
            auto parameter = (name.empty() ? node : GetNode(node, name));
            return YamlReader::ReadNodeSafe(parameter, val, default_value);
        }

        template<typename T>
        inline T YamlReader::ReadNode(const cv::FileNode &node)
        {
            if(node.empty() || node.isNone())
            {
#ifdef YAML_RAISE_EXCEPTION_IF_RETURN_NULL_NODE
                throw Exception(Exception::ERROR_YAML, "Yaml node is null.");
#else
                return T();
#endif
            }
            return ReadNodeSafe(node, T());
        }

        template<typename T>
        inline bool YamlReader::ReadNode(const cv::FileNode &node, T &val)
        {
            T val_old = val;
            if(!node.empty() && !node.isNone())
            {
                cv::readFileNode(node, val, T());
                return true;
            }
            val = val_old;
            return false;
        }

        template<typename T>
        inline T YamlReader::ReadNodeSafe(const cv::FileNode &node, const T &default_value)
        {
            T value;
            cv::readFileNode(node, value, default_value);
            return value;
        }

        template<typename T>
        inline bool YamlReader::ReadNodeSafe(const cv::FileNode &node, T &val, const T &default_value)
        {
            return cv::readFileNode(node, val, default_value);
        }
#endif

        template<typename T>
        inline void YamlWriter::Write(const std::string &name, const T &value)
        {
            SaveNode<T>(m_out, name, value);
        }

#ifdef SLAM_HAVE_YAML
        template<typename T>
        inline void YamlWriter::SaveNode(YAML::Emitter &yaml, const std::string &name, const T &value)
        {
            yaml << YAML::BeginMap;
            yaml << YAML::Key << name;
            yaml << YAML::Value << value;
            yaml << YAML::EndMap;
        }
#else
        template<typename T>
        inline void YamlWriter::SaveNode(cv::FileStorage &yaml, const std::string &name, const T &value)
        {
            cv::write(yaml, name, value);
        }
#endif
    }
}
#endif //SLAM_YAML_H
