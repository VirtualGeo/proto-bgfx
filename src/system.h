#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>

static std::string GetBaseDir(const std::string& filepath)
{
    std::string ret = "";
    if (filepath.find_last_of("/\\") != std::string::npos)
        ret = filepath.substr(0, filepath.find_last_of("/\\"));
//    return "";

    if (ret.empty()) {
        ret = ".";
    }
#ifdef _WIN32
    ret += "\\";
#else
    ret += "/";
#endif
    return ret;
}

static bool FileExists(const std::string& abs_filename)
{
    bool ret;
    FILE* fp = fopen(abs_filename.c_str(), "rb");
    if (fp) {
        ret = true;
        fclose(fp);
    } else {
        ret = false;
    }

    return ret;
}

#endif // SYSTEM_H
