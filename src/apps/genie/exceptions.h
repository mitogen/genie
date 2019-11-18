// Copyright 2018 The genie authors

/**
 *  @file exceptions.h
 *  @brief Exceptions interface
 *  @author Jan Voges
 *  @bug No known bugs
 */

#ifndef DSG_COMMON_EXCEPTIONS_H_
#define DSG_COMMON_EXCEPTIONS_H_

#include <exception>
#include <iostream>
#include <string>

#include "utilities.h"

namespace dsg {
namespace common {

class Exception : public std::exception {
   public:
    explicit Exception(const std::string& message);

    virtual ~Exception(void) throw();

    //to review
    Exception(const Exception&) = default; //raised an error in CLion below when deleted
    Exception& operator=(const Exception&) = default;
    Exception(Exception&&) = default;
    Exception& operator=(Exception&&) = default;

    virtual std::string getMessage(void) const;

    virtual const char* what(void) const throw();

   protected:
    std::string m_message;
};

class RuntimeErrorException : public Exception {
   public:
    explicit RuntimeErrorException(const std::string& message);

    // TODO Jan check these
    virtual ~RuntimeErrorException() = default;
    RuntimeErrorException(const RuntimeErrorException&) = delete;
    RuntimeErrorException& operator=(const RuntimeErrorException&) = delete;
    RuntimeErrorException(RuntimeErrorException&&) = default; //setting this to delete caused a CLion error below
    RuntimeErrorException& operator=(RuntimeErrorException&&) = delete;
};

inline void throwRuntimeError(const std::string& message) {
    std::cout.flush();
    throw RuntimeErrorException(message);
}

class RuntimeErrorReporter {
   public:
    RuntimeErrorReporter(const std::string& file, const std::string& function, const int line);

    // TODO Jan check these
    virtual ~RuntimeErrorReporter() = default;
    RuntimeErrorReporter(const RuntimeErrorReporter&) = delete;
    RuntimeErrorReporter& operator=(const RuntimeErrorReporter&) = delete;
    RuntimeErrorReporter(RuntimeErrorReporter&&) = delete;
    RuntimeErrorReporter& operator=(RuntimeErrorReporter&&) = delete;

    void operator()(const std::string& message) {
        std::string s = dateTime() + " ";
        s += fileBaseName(m_file) + ":";
        s += m_function + ":";
        s += std::to_string(m_line) + ": ";
        s += message;

        throwRuntimeError(s);
    }

   private:
    std::string m_file;
    std::string m_function;
    int m_line;
};

}  // namespace common
}  // namespace dsg

// Remove the symbol for the function, then define a new version that instead
// creates a stack temporary instance initialized with the caller.
#undef throwRuntimeError
#define throwRuntimeError dsg::common::RuntimeErrorReporter(__FILE__, __FUNCTION__, __LINE__)

#endif  // DSG_COMMON_EXCEPTIONS_H_
