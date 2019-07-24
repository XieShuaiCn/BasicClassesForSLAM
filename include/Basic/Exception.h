//
// Created by xieshuai on 19-3-11.
//

#ifndef SLAM_EXCEPTION_H
#define SLAM_EXCEPTION_H

#include <string>
#include <exception>

namespace SLAM
{

    class Exception : public std::exception
    {
    public:
        enum ErrorType
        {
            // Success, no error
                    ERROR_NO = 0,
            // Assert failed
                    ERROR_ASSERT,
            // Regular exception, SLAM error
                    ERROR_EXCEPT,
            // std::exception or error in namespace std
                    ERROR_STD,
            // error in boost library
                    ERROR_BOOST,
            // error in eigen library
                    ERROR_EIGEN,
            // error in opencv library
                    ERROR_CV,
            // error in yaml library
                    ERROR_YAML,
            // error in ceres library
                    ERROR_CERES,
            // error in sophus library
                    ERROR_SOPHUS,
            // error in opengl library
                    ERROR_GL,
            // Unknown error
                    ERROR_UNKNOWN
        };
        Exception() noexcept;

        explicit Exception(const std::string &_msg) noexcept;

        Exception(ErrorType _code, const std::string &_msg) noexcept;

        Exception(ErrorType _code, std::string _msg, std::string _func, std::string _file, int _line) noexcept;

        Exception(ErrorType _code, const char *_msg, const char *_func, const char *_file, int _line) noexcept;

        Exception(const Exception &ex) noexcept;

        Exception(Exception &&ex) noexcept;

        ~Exception() noexcept override = default;

        const std::string &Message() const noexcept;

        const char *what() const noexcept override;

    protected:

        void FormatMessage() noexcept;

        std::string msg; ///< the formatted error message

        ErrorType code; ///< error code
        std::string err; ///< error description
        std::string func; ///< function name. Available only when the compiler supports getting it
        std::string file; ///< source file name where the error has occurred
        int line; ///< line number in the source file where the error has occurred
    };
}

#endif //SLAM_EXCEPTION_H
