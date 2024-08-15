// Implement parsing in the ini file

#include <string>
#include <variant>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <err.hpp>
#include <ini.hpp>

using namespace acbs;
using namespace acbs::ini;

std::variant<Project, err::AcbsErr> Project::parse(const std::string &fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        return err::AcbsErr { .type = err::AcbsErrType::NoIniFile, .extraInfo = fileName };
    }

    // Defaults
    std::string compilerCompiler =      "gcc";
    std::string compilerFlags =         "-O2 -Wall -Werror";
    std::string compilerDebugFlags =    "-g";
    std::string compilerLinker =        "gcc";
    std::string compilerLinkerFlags =   "";
    std::string projectName =           "";
    bool projectIsBin =                 true;
    bool projectIsCpp =                 false;
    auto projectInclude =               std::vector { std::string("include") };
    auto projectSource =                std::vector { std::string("src") };
    std::string projectBuild =          "obj";

    // Parse to replace defaults
    std::string section = "";
    std::string line;
    size_t ln = 1;
    while (std::getline(file, line)) {
        // Check for a new section
        if (line[0] == '[') {
            if (line[line.length() - 1] != ']') {
                return err::AcbsErr {
                    .type = err::AcbsErrType::MalformedSection,
                    .extraInfo = line
                };
            }
            if (line != "[project]"
                    && line != "[compiler]") {
                return err::AcbsErr {
                    .type = err::AcbsErrType::MalformedSection,
                    .extraInfo = line
                };
            }
            section = line.substr(1, line.length() - 2);
            ln++;
            continue;
        }

        // Check for empty
        bool empty = true;
        for (size_t i = 0; i < line.length(); i++) {
            if (line[i] != ' '
                    && line[i] != '\t'
                    && line[i] != '\r'
                    && line[i] != '\n') {
                empty = false;
                break;
            }
        }
        if (empty) {
            ln++;
            continue;
        }

        // Split key value pair
        size_t pos = 0;
        while (pos < line.length() && (line[pos] == ' ' || line[pos] == '\t')) {
            pos++;
        }
        std::stringstream key;
        while (pos < line.length()
                && line[pos] != '='
                && line[pos] != ' '
                && line[pos] != '\t') {
            key << line[pos];
            pos++;
        }
        while (pos < line.length() && (line[pos] == ' ' || line[pos] == '\t')) {
            pos++;
        }
        if (pos >= line.length() || line[pos] != '=') {
            return err::AcbsErr {
                .type = err::AcbsErrType::NoKey,
                .extraInfo = std::to_string(ln)
            };
        }
        pos++;
        if (section == "") {
            return err::AcbsErr {
                .type = err::AcbsErrType::NoKey,
                .extraInfo = std::to_string(ln)
            };
        }
        while (pos < line.length() && (line[pos] == ' ' || line[pos] == '\t')) {
            pos++;
        }
        std::stringstream value;
        while (pos < line.length()) {
            value << line[pos];
            pos++;
        }
        if (section == "compiler") {
            if (key.str() == "compiler") {
                compilerCompiler = value.str();
            } else if (key.str() == "flags") {
                compilerFlags = value.str();
            } else if (key.str() == "debugflags") {
                compilerDebugFlags = value.str();
            } else if (key.str() == "linker") {
                compilerLinker = value.str();
            } else if (key.str() == "linkerflags") {
                compilerLinkerFlags = value.str();
            } else {
                return err::AcbsErr {
                    .type = err::AcbsErrType::UnknownKey,
                    .extraInfo = section + "." + key.str() + " (line " + std::to_string(ln) + ")"
                };
            }
        } else if (section == "project") {
            if (key.str() == "name") {
                projectName = value.str();
            } else if (key.str() == "isbin") {
                std::string val = value.str();
                while (val.size() && std::isspace(val.front())) {
                    val.erase(val.begin());
                }
                while (val.size() && isspace(val.back())) {
                    val.pop_back();
                }
                if (val == "true") {
                    projectIsBin = true;
                } else if (val == "false") {
                    projectIsBin = false;
                } else {
                    return err::AcbsErr {
                        .type = err::AcbsErrType::BadValue,
                        .extraInfo = val + " (line " + std::to_string(ln) + ")"
                    };
                }
            } else if (key.str() == "iscpp") {
                std::string val = value.str();
                while (val.size() && std::isspace(val.front())) {
                    val.erase(val.begin());
                }
                while (val.size() && isspace(val.back())) {
                    val.pop_back();
                }
                if (val == "true") {
                    projectIsCpp = true;
                } else if (val == "false") {
                    projectIsCpp = false;
                } else {
                    return err::AcbsErr {
                        .type = err::AcbsErrType::BadValue,
                        .extraInfo = val + " (line " + std::to_string(ln) + ")"
                    };
                }
            } else if (key.str() == "include") {
                std::string val = value.str();
                std::vector<std::string> incs;
                size_t pos = 0;
                std::stringstream inc;
                while (pos < val.length()) {
                    if (val[pos] == ',') {
                        incs.push_back(inc.str());
                        inc.str("");
                        pos++;
                        continue;
                    }
                    inc << val[pos];
                    pos++;
                }
                incs.push_back(inc.str());
                projectInclude = incs;
            } else if (key.str() == "source") {
                std::string val = value.str();
                std::vector<std::string> srcs;
                size_t pos = 0;
                std::stringstream src;
                while (pos < val.length()) {
                    if (val[pos] == ',') {
                        srcs.push_back(src.str());
                        src.str("");
                        pos++;
                        continue;
                    }
                    src << val[pos];
                    pos++;
                }
                srcs.push_back(src.str());
                projectSource = srcs;
            } else if (key.str() == "build") {
                projectBuild = value.str();
            }  else {
                return err::AcbsErr {
                    .type = err::AcbsErrType::UnknownKey,
                    .extraInfo = section + "." + key.str() + " (line " + std::to_string(ln) + ")"
                };
            }
        } else {
            return err::AcbsErr {
                .type = err::AcbsErrType::NoKey,
                .extraInfo = std::to_string(ln)
            };
        }
    }
    file.close();

    // Everything has a default. Only one that needs a value is project name
    if (projectName == "") {
        return err::AcbsErr { .type = err::AcbsErrType::NoProjectName, .extraInfo = "" };
    }

    // Build project
    return Project {
        .compiler = CompilerSection {
            .compiler = compilerCompiler,
            .flags = compilerFlags,
            .debugFlags = compilerDebugFlags,
            .linker = compilerLinker,
            .linkerFlags = compilerLinkerFlags
        }, .project = ProjectSection {
            .name = projectName,
            .isBin = projectIsBin,
            .isCpp = projectIsCpp,
            .include = projectInclude,
            .source = projectSource,
            .build = projectBuild
        }
    };
}

