// Error codes for the project

#pragma once

#include <string>

namespace acbs {
    namespace err {
        enum class AcbsErr {
            NoArgumentsProvided,
            TooManyArguments,
            InvalidCommand,
            NoIniFile
        };

        inline std::string errToString(const AcbsErr &err) {
            switch (err) {
                default:
                case AcbsErr::NoArgumentsProvided:
                    return "No arguments were provided to the acbs command.";
                case AcbsErr::TooManyArguments:
                    return "ACBS expects no more than 2 arguments!";
                case AcbsErr::InvalidCommand:
                    return "Unknown command provided";
                case AcbsErr::NoIniFile:
                    return "No acbs.ini in folder";
            }
        }
    }
}

