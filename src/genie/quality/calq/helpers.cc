#include "helpers.h"

// -----------------------------------------------------------------------------

#include <fstream>

// -----------------------------------------------------------------------------

#include "error_reporter.h"

// -----------------------------------------------------------------------------

namespace calq {

// -----------------------------------------------------------------------------

std::string currentDateAndTime(){
    // ISO 8601 format: 2007-04-05T14:30:21Z
    char timeString[] = "yyyy-mm-ddTHH:MM:SSZ";

    time_t currentTime = time(nullptr);
    if (currentTime == ((time_t) -1)) {
        throwErrorException("time failed");
    }
    struct tm timeinfo{0,
                       0,
                       0,
                       1,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       nullptr};

#ifdef _WIN32
    errno_t err = gmtime_s(&timeinfo, &currentTime);
    if (err != 0) {
        throwErrorException("gmtime_s failed");
    }
#else
    struct tm *ret = gmtime_r(&currentTime, &timeinfo);
    if (ret == nullptr) {
        throwErrorException("gmtime_r failed");
    }
#endif

    if (strftime(
            timeString,
            sizeof(timeString),
            "%Y-%m-%dT%H:%M:%SZ",
            &timeinfo
    ) == 0) {
        throwErrorException("strftime failed");
    }

    std::string result(timeString);
    return result;
}

// -----------------------------------------------------------------------------

bool fileExists(const std::string& path){
    if (path.empty()) {
        throwErrorException("path is empty");
    }
    std::ifstream ifs(path.c_str());
    return ifs.good();
}

// -----------------------------------------------------------------------------

std::string fileBaseName(const std::string& path){
    if (path.empty()) {
        throwErrorException("path is empty");
    }
    std::string const& delims = "/\\";
    return path.substr(path.find_last_of(delims) + 1);
}

// -----------------------------------------------------------------------------

std::string fileNameExtension(const std::string& path){
    if (path.empty()) {
        throwErrorException("path is empty");
    }
    if (path.find_last_of('.') != std::string::npos) {
        return path.substr(path.find_last_of('.') + 1);
    }
    return "";
}

// -----------------------------------------------------------------------------

std::string removeFileNameExtension(const std::string& path){
    if (path.empty()) {
        throwErrorException("path is empty");
    }
    std::string::size_type const p(path.find_last_of('.'));
    return p > 0 && p != std::string::npos ? path.substr(0, p) : path;
}

// -----------------------------------------------------------------------------

}  // namespace calq

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
