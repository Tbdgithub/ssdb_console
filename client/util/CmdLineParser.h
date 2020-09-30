//
// Created by john on 2020/9/30.
//

#ifndef SSDB_CONSOLE_CMDLINEPARSER_H
#define SSDB_CONSOLE_CMDLINEPARSER_H

#include <string>

using namespace std;

class CmdLineParser {
private:

    vector<string> parts;
public:

    CmdLineParser() {

    }

    vector<std::string> *parse_cmd(std::string str);

    static inline bool isEmptyChar(char c) {
        return c == ' ' || c == '\t';
    }

    static inline std::string trim_line(std::string source) {
        std::string result = source.erase(source.find_last_not_of("\n") + 1);
        return result;
    }

};

#endif //SSDB_CONSOLE_CMDLINEPARSER_H
