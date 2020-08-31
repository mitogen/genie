#include "exceptions.h"

// -----------------------------------------------------------------------------

namespace genie{
namespace quality{
namespace calq {

// -----------------------------------------------------------------------------

Exception::Exception(const std::string& msg)
        : msg_(msg){
}

// -----------------------------------------------------------------------------

Exception::Exception(const Exception& e) noexcept
        : msg_(e.msg_){
}

// -----------------------------------------------------------------------------

Exception::~Exception() noexcept = default;

// -----------------------------------------------------------------------------

std::string Exception::getMessage() const{
    return msg_;
}

// -----------------------------------------------------------------------------

const char *Exception::what() const noexcept{
    return msg_.c_str();
}

// -----------------------------------------------------------------------------

}  // namespace calq
}  // namespace quality
}  // namespace genie

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
