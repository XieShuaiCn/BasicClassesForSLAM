//
// Created by wqy on 18-1-11.
//

#ifndef SLAM_CONFIGURE_H
#define SLAM_CONFIGURE_H

#include <opencv2/core.hpp>

namespace SLAM
{
    class Configure
    {
    public:
        //EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

        Configure() = default;

        // Get a instance of 'Configure'
        static Configure &GetInstance();

        // the short name of GetInstance
        static Configure &I();

        //instrinsic
        float CameraFx() const;

        float CameraFy() const;

        float CameraCx() const;

        float CameraCy() const;
        
        bool LoadConfig(const std::string &file);

    protected:

        float m_dFx = 0.f, m_dFy = 0.f, m_dCx = 0.f, m_dCy = 0.f;
    };

    /////////////////////////////////////////////////////////

    // the short name of GetInstance
    inline Configure &Configure::I()
    {
        return Configure::GetInstance();
    }

    inline float Configure::CameraFx() const
    {
        return m_dFx;
    }

    inline float Configure::CameraFy() const
    {
        return m_dFy;
    }

    inline float Configure::CameraCx() const
    {
        return m_dCx;
    }

    inline float Configure::CameraCy() const
    {
        return m_dCy;
    }
}
#endif //SLAM_CONFIGURE_H
