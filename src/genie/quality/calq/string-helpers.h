#ifndef CALQ_STRING_HELPERS_H_
#define CALQ_STRING_HELPERS_H_

#include <string>

namespace calq {

inline std::string &rtrim(std::string &s, const char *t = " \t\n\r\f\v") {
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

inline std::string &ltrim(std::string &s, const char *t = " \t\n\r\f\v") {
    s.erase(0, s.find_first_not_of(t));
    return s;
}

inline std::string &trim(std::string &s, const char *t = " \t\n\r\f\v") { return ltrim(rtrim(s, t), t); }

}  // namespace calq

#endif  // CALQ_STRING_HELPERS_H_
