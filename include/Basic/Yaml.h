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
            YamlWriter();

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

        class YamlIO : public YamlReader
        {
        public:
            YamlIO() = default;

            explicit YamlIO(const std::string &file);

            ~YamlIO();

            std::string GetString();

            template<typename T>
            void Write(const std::string &name, const T &value);
        private:
            std::string m_strFile;
        };
    }
}

#ifdef SLAM_HAVE_YAML
namespace YAML
{
    // output std::initializer_list to YAML::Emitter
    template<typename _T>
    Emitter &operator<<(Emitter &out, const std::initializer_list<_T> &lst);
}
#else //SLAM_HAVE_OPENCV
namespace cv{
    bool readFileNode_bool(const FileNode& node, bool& value, bool default_value);
    bool readFileNode_long(const FileNode& node, long& value, long default_value);
    bool readFileNode_ulong(const FileNode& node, unsigned long& value, unsigned long default_value);

    template <typename _Tp>
    struct NodeConverter{
        static bool read(const FileNode &node, _Tp &value, const _Tp &default_value){
            if(node.empty() || node.isNone())
                return false;
            cv::read(node, value, default_value);
            return true;
        }
        static bool write(FileStorage &file, const String& name, const _Tp &value) {
            cv::write(file, name, value);
            return true;
        }
    };

    template <>
    struct NodeConverter<bool>{
        static bool read(const FileNode &node, bool &value, const bool &default_value){
            return readFileNode_bool(node, value, default_value);
        }
        static bool write(FileStorage &file, const String& name, const bool &value) {
            cv::write(file, name, (value ? "true" : "false"));
            return true;
        }
    };

    template <>
    struct NodeConverter<unsigned int>{
        static bool read(const FileNode &node, unsigned int &value, const unsigned int &default_value){
            unsigned long val = 0;
            bool ret = readFileNode_ulong(node, val, static_cast<unsigned long>(default_value));
            value = static_cast<unsigned int>(val);
            return ret;
        }
        static bool write(FileStorage &file, const String& name, const unsigned int &value) {
            cv::write(file, name, std::to_string(value));
            return true;
        }
    };

    template <>
    struct NodeConverter<long>{
        static bool read(const FileNode &node, long &value, const long &default_value){
            return readFileNode_long(node, value, default_value);
        }
        static bool write(FileStorage &file, const String& name, const long &value) {
            cv::write(file, name, std::to_string(value));
            return true;
        }
    };

    template <>
    struct NodeConverter<unsigned long>{
        static bool read(const FileNode &node, unsigned long &value, const unsigned long &default_value){
            return readFileNode_ulong(node, value, default_value);
        }
        static bool write(FileStorage &file, const String& name, const unsigned long &value) {
            cv::write(file, name, std::to_string(value));
            return true;
        }
    };
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
            if(!parameter.IsDefined() || parameter.IsNull())
            {
#ifdef YAML_RAISE_EXCEPTION_IF_RETURN_NULL_NODE
                throw Exception(Exception::ERROR_YAML, "Yaml node has not this key.");
#else
                return T();
#endif
            }
            return parameter.as<T>();
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
            try {
                if (node.IsDefined() && !node.IsNull()) {
                    val = node.as<T>();
                    return true;
                }
            }catch (...){ }
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
            try {
                if(node.IsDefined() && !node.IsNull())
                {
                    val = node.as<T>();
                    return true;
                }
            }catch (...){ }
            val = default_value;
            return false;
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
                return cv::NodeConverter<T>::read(node, val, T());
            }
            val = val_old;
            return false;
        }

        template<typename T>
        inline T YamlReader::ReadNodeSafe(const cv::FileNode &node, const T &default_value)
        {
            T value;
            cv::NodeConverter<T>::read(node, value, default_value);
            return value;
        }

        template<typename T>
        inline bool YamlReader::ReadNodeSafe(const cv::FileNode &node, T &val, const T &default_value)
        {
            return cv::NodeConverter<T>::read(node, val, default_value);
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
            yaml << YAML::Key << name;
            yaml << YAML::Value << value;
        }
#else
        template<typename T>
        inline void YamlWriter::SaveNode(cv::FileStorage &yaml, const std::string &name, const T &value)
        {
            cv::NodeConverter<T>::write(yaml, name, value);
        }
#endif

#ifdef SLAM_HAVE_YAML
        template<typename T>
        inline void YamlIO::Write(const std::string &name, const T &value)
        {
            m_root[name] = value;
        }
#else
        template<typename T>
        inline void YamlIO::Write(const std::string &name, const T &value)
        {
            m_root[name] = value;
        }
#endif
    }
}
#endif //SLAM_YAML_H
