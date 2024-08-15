// Error codes for the project

#pragma once

#include <string>

namespace acbs {
    namespace err {
        enum class AcbsErrType {
            NoArgumentsProvided,
            TooManyArguments,
            InvalidCommand,
            NoIniFile,
            NoProjectName,
            MalformedSection,
            NoKey,
            KeyOutsideSection,
            UnknownKey,
            BadValue
        };

        struct AcbsErr {
            const AcbsErrType type;
            const std::string extraInfo;
        };

        inline std::string errToString(const AcbsErr &err) {
            switch (err.type) {
                default:
                case AcbsErrType::NoArgumentsProvided:
                    return "No arguments were provided to the acbs command.";
                case AcbsErrType::TooManyArguments:
                    return "ACBS expects no more than 2 arguments!";
                case AcbsErrType::InvalidCommand:
                    return "Unknown command '" + err.extraInfo + "'provided";
                case AcbsErrType::NoIniFile:
                    return "No such '" + err.extraInfo + "'";
                case AcbsErrType::NoProjectName:
                    return "No project name in acbs.ini";
                case AcbsErrType::MalformedSection:
                    return "Failed to parse section name '" + err.extraInfo + "'";
                case AcbsErrType::NoKey:
                    return "No key in ini on line " + err.extraInfo;
                case AcbsErrType::KeyOutsideSection:
                    return "Key pair provided outside of a section on line " + err.extraInfo;
                case AcbsErrType::UnknownKey:
                    return "Unknown key in ini file '" + err.extraInfo + "'";
                case AcbsErrType::BadValue:
                    return "Bad value in ini file '" + err.extraInfo + "'";
            }
        }
    }
}

