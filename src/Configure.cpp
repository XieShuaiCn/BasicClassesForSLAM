//
// Created by xieshuai on 19-3-13.
//
#include "Configure.h"
#include "Basic/Yaml.h"

namespace SLAM
{

    Configure &Configure::GetInstance()
    {
        static Configure s_conf;
        return s_conf;
    }


    bool Configure::LoadConfig(const std::string &file)
    {
        //cv::FileStorage storage(file, cv::FileStorage::READ);
        Basic::YamlReader yamlReader(file);
        if (!yamlReader.IsLoaded()) return false;

        //camera intrinsic
        m_dFx = yamlReader.Read<float>("Camera.fx");
        yamlReader.Read("Camera.fy", m_dFy);
        m_dCx = yamlReader.ReadSafe("Camera.cx", m_dCx);
        yamlReader.ReadSafe("Camera.cy", m_dCy, 256.f);
        return true;
    }
}
