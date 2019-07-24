//
// Created by xieshuai on 19-3-14.
//
#include "Basic/Exception.h"

namespace SLAM
{

    const char *const g_code_string[] = {"Success",
                                         "Assert",
                                         "Exception",
                                         "StdError",
                                         "BoostError",
                                         "EigenError",
                                         "OpenCVError",
                                         "YamlError",
                                         "CeresError",
                                         "SophusError",
                                         "OpenGLError",
                                         "Unknown"};

    static_assert(sizeof(g_code_string) / sizeof(char *) == (1 + Exception::ERROR_UNKNOWN),
                  "The size of code strings is not the same as exception code.");

    Exception::Exception() noexcept
            : code(ERROR_EXCEPT),
              line(-1)
    {
    }

    Exception::Exception(const std::string &_msg) noexcept
            : msg(_msg),
              code(ERROR_EXCEPT),
              err(_msg),
              line(-1)
    {
    }

    Exception::Exception(ErrorType _code, const std::string &_msg) noexcept
            : msg(_msg),
              code(_code),
              err(_msg),
              line(-1)
    {
    }

    Exception::Exception(ErrorType _code, std::string _msg, std::string _func, std::string _file, int _line) noexcept
            : code(_code),
              err(std::move(_msg)),
              func(std::move(_func)),
              file(std::move(_file)),
              line(_line)
    {
        FormatMessage();
    }

    Exception::Exception(ErrorType _code, const char *_msg, const char *_func, const char *_file, int _line) noexcept
            : code(_code),
              err(_msg),
              func(_func),
              file(_file),
              line(_line)
    {
        FormatMessage();
    }

    Exception::Exception(const SLAM::Exception &ex) noexcept
            : msg(ex.msg),
              code(ex.code),
              err(ex.err),
              func(ex.func),
              file(ex.file),
              line(ex.line)
    {
    }

    Exception::Exception(SLAM::Exception &&ex) noexcept
            : msg(std::move(ex.msg)),
              code(ex.code),
              err(std::move(ex.err)),
              func(std::move(ex.func)),
              file(std::move(ex.file)),
              line(ex.line)
    {
    }

    void Exception::FormatMessage() noexcept
    {
        try {
            code = (0 < code && code < ERROR_UNKNOWN ? code : ERROR_UNKNOWN);
            msg.append("[ ");
            msg.append(file);
            msg.push_back(':');
            msg.append(std::to_string(line));
            msg.append(" ] ");
            msg.append(g_code_string[code]);
            msg.append(": ");
            msg.append(msg);
        } catch (...) {}
    }

    const std::string &Exception::Message() const noexcept
    {
        return msg;
    }

    const char *Exception::what() const noexcept
    {
        return err.c_str();
    }

}
