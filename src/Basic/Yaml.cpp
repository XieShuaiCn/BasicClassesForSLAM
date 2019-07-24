//
// Created by xieshuai on 19-5-24.
//
#include "Basic/Yaml.h"
#include <string>
#include <fstream>
#ifdef SLAM_HAVE_YAML
#include <yaml-cpp/yaml.h>
#endif
#ifdef SLAM_HAVE_BOOST
#include <boost/filesystem/operations.hpp>
#endif
#ifdef SLAM_HAVE_OPENCV
#include <opencv2/core/types_c.h>
#endif
#include <Basic/Exception.h>

namespace SLAM
{
    namespace Basic
    {
#ifdef SLAM_HAVE_YAML
        YamlReader::YamlReader(const YAML::Node &node)
        {
            m_root = node;
        }
#else
        YamlReader::YamlReader(const cv::FileStorage &root)
        {
            m_file = root;
            m_root = m_file.root();
        }

        YamlReader::YamlReader(const cv::FileNode &node)
        {
            m_root = node;
        }
#endif

        YamlReader::YamlReader(const std::string &yaml)
        {
#ifdef SLAM_HAVE_BOOST
            bool bFileExist = yaml.size() < 512 && boost::filesystem::is_regular_file(yaml);
#else
            bool bFileExist = yaml.size() < 512;
            if(bFileExist) {
                FILE *fp = fopen(yaml.c_str(), "r");
                bFileExist = (fp != nullptr);
                if (fp) fclose(fp);
            }
#endif
            if (bFileExist)
            {
                LoadFile(yaml);
            }
            else
            {

                LoadString(yaml);
            }
        }

        void YamlReader::LoadFile(const std::string &file)
        {
            try
            {
#ifdef SLAM_HAVE_YAML
                m_root = YAML::LoadFile(file);
#else
                m_file.open(file, cv::FileStorage::READ | cv::FileStorage::FORMAT_YAML);
                if(m_file.isOpened()) {
                    m_root = m_file.root();
                }
#endif
            }
#ifdef SLAM_HAVE_YAML
            catch (YAML::Exception &e)
            {
                throw SLAM::Exception(Exception::ERROR_YAML, e.msg);
            }
#else
            catch(cv::Exception &e)
            {
                throw SLAM::Exception(Exception::ERROR_CV, std::string(e.msg));
            }
#endif
            catch (...)
            {
                throw SLAM::Exception(Exception::ERROR_EXCEPT, "Can not load yaml file. =>" + file);
            }
        }

        void YamlReader::LoadString(const std::string &content)
        {
#ifdef SLAM_HAVE_YAML
            try
            {
                m_root = YAML::Load(content);
            }
            catch (YAML::Exception &e)
            {
                throw SLAM::Exception(Exception::ERROR_YAML, e.msg);
            }
            catch (...)
            {
                throw SLAM::Exception(Exception::ERROR_EXCEPT, "Can not load yaml strings.");
            }
#else
            (void)content;
            throw SLAM::Exception(Exception::ERROR_EXCEPT, std::string("Can not call ") + CV_Func+ " without Yaml-cpp.");
#endif
        }

#ifdef SLAM_HAVE_YAML
        const YAML::Node YamlReader::GetNode(const YAML::Node &node, const std::string &name)
        {
            if (!node.IsDefined() || node.IsNull()) {
                return node;
            }
            size_t it1 = name.find(':', 0);
            if (it1 == std::string::npos) {
                return node[name];
            }
            std::string n = name.substr(0, it1);
            return GetNode(node[n], name.substr(it1 + 1));
        }

        const YAML::Node YamlReader::GetNode(const std::string &name) const
        {
            size_t it0 = 0;
            size_t it1 = name.find(':');
            YAML::Node node = m_root;
            while(it1 < std::string::npos && node.IsDefined()){
                if(it0 < it1) {
                    std::string n = name.substr(it0, it1 - it0);
                    node = node[n];
                }
                it0 = it1+1;
                it1 = name.find(':', it0);
            }
            if(it0 < name.size() && node.IsDefined())
                return node[name.substr(it0)];
            else
                return YAML::Node();
        }
#else
        const cv::FileNode YamlReader::GetNode(const cv::FileNode &node, const std::string &name)
        {
            if (node.empty() || node.isNone()) {
                return node;
            }
            size_t it1 = name.find(':', 0);
            if (it1 == std::string::npos) {
                return node[name];
            }
            std::string n = name.substr(0, it1);
            return GetNode(node[n], name.substr(it1 + 1));
        }

        const cv::FileNode YamlReader::GetNode(const std::string &name) const
        {
            size_t it0 = 0;
            size_t it1 = name.find(':');
            auto node = m_root;
            while(it1 < std::string::npos && !node.empty()){
                if(it0 < it1) {
                    std::string n = name.substr(it0, it1 - it0);
                    node = node[n];
                }
                it0 = it1+1;
                it1 = name.find(':', it0);
            }
            if(it0 < name.size() && !node.empty())
                return node[name.substr(it0)];
            else
                return cv::FileNode();
        }
#endif // SLAM_HAVE_YAML

#ifdef SLAM_HAVE_YAML
        YamlWriter::YamlWriter(const std::string &file)
        {
            m_strFile = file;
        }

        YamlWriter::YamlWriter(std::ostream &ostream) :
                m_out(ostream)
        {
        }

        YamlWriter::~YamlWriter()
        {
            std::ofstream of(m_strFile);
            if(of) {
                of << m_out.c_str();
                of.close();
            }
        }

        std::string YamlWriter::GetString()
        {
            return std::string(m_out.c_str());
        }
#else
        YamlWriter::YamlWriter(const std::string &file)
        {
            m_out.open(file, cv::FileStorage::WRITE);
        }

        YamlWriter::YamlWriter(std::ostream &ostream)
        {
            (void)ostream;
            throw SLAM::Exception(SLAM::Exception::ERROR_EXCEPT, "Please build with yaml-cpp");
        }

        YamlWriter::~YamlWriter()
        {
            m_out.release();
        }

        std::string YamlWriter::GetString()
        {
            return std::string(m_out.releaseAndGetString());
        }
#endif

        YamlIO::YamlIO(const std::string &file) :
                YamlReader(file), YamlWriter(file)
        {

        }

        YamlIO::YamlIO(std::iostream &stream) :
                YamlReader(), YamlWriter(stream)
        {
            if (stream)
            {
                std::stringstream buffer;
                buffer << stream.rdbuf();
                YamlReader::LoadString(buffer.str());
            }
        }
    }
}

#ifdef SLAM_HAVE_YAML
namespace YAML
{
#ifdef SLAM_HAVE_OPENCV
    // convert 'YAML::Node' to 'template Mat'
    template<typename _Ty>
    cv::Mat ConvertToMatT(const Node &node, int rows, int cols)
    {
        cv::Mat __mat(rows, cols, SLAM::Basic::TypeFromStd<_Ty>::CvCode);
        for (int i = 0; i < rows; ++i)
        {
            _Ty *p = (_Ty *) (__mat.ptr(i));
            for (int j = 0; j < cols; ++j)
            {
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
        for (int i = 0; i < mat.rows; ++i)
        {
            _Ty *p = (_Ty *) (mat.ptr(i));
            for (int j = 0; j < mat.cols; ++j)
            {
                node_data.push_back<_Ty>(*(p + j));
            }
        }
        node["data"] = node_data;
    }

    // Specific template of cv::Mat converter
    Node convert<cv::Mat>::encode(const cv::Mat &m)
    {
        Node node;
        switch (m.depth())
        {
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
        if (!node.IsMap() || node.size() != 4)
        {
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
        switch (dt)
        {
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

    template struct convert<Eigen::Matrix<double, 3, 1>>;
#endif
}
#else
namespace cv
{

    bool readFileNode_bool(const FileNode &node, bool &value, bool default_value)
    {
        if(node.empty() || node.isNone()) {
            // copy data if they are not the same memory address.
            value = default_value;
            return false;
        }
        if (CV_NODE_TYPE(node.node->tag) == CV_NODE_STRING) {
            std::string buffer(node.node->data.str.ptr, static_cast<size_t>(node.node->data.str.len));
            static const struct
            {
                std::string truename, falsename;
            } names[] = {
                    {"y",    "n"},
                    {"yes",  "no"},
                    {"true", "false"},
                    {"on",   "off"},
            };
            for (char &ch : buffer) {
                if (std::isalpha(ch)) ch |= 0x20;
            }
            for (unsigned i = 0; i < sizeof(names) / sizeof(names[0]); ++i) {
                if (names[i].truename == buffer) {
                    value = true;
                    return true;
                }

                if (names[i].falsename == buffer) {
                    value = false;
                    return true;
                }
            }
            return false;
        }
        else if (CV_NODE_TYPE(node.node->tag) == CV_NODE_INTEGER) {
            value = (node.node->data.i != 0);
            return true;
        }
        else{
            value = default_value;
            return false;
        }
    }

    bool readFileNode_long(const FileNode &node, long &value, long default_value)
    {
        if(node.empty() || node.isNone()) {
            // copy data if they are not the same memory address.
            value = default_value;
            return false;
        }
        if (CV_NODE_TYPE(node.node->tag) == CV_NODE_INTEGER) {
            value = node.node->data.i;
            return true;
        }
        else if(CV_NODE_TYPE(node.node->tag) == CV_NODE_FLOAT){
            value = static_cast<long>(node.node->data.f);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool readFileNode_ulong(const FileNode& node, unsigned long& value, unsigned long default_value)
    {
        if(node.empty() || node.isNone()) {
            // copy data if they are not the same memory address.
            value = default_value;
            return false;
        }
        if (CV_NODE_TYPE(node.node->tag) == CV_NODE_INTEGER) {
            value = static_cast<unsigned long>(node.node->data.i);
            return true;
        }
        else if(CV_NODE_TYPE(node.node->tag) == CV_NODE_FLOAT){
            value = static_cast<unsigned long>(node.node->data.f);
            return true;
        }
        else
        {
            return false;
        }
    }
}
#endif // SLAM_HAVE_YAML
