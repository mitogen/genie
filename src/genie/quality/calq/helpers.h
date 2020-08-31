#ifndef CALQAPP_HELPERS_H_
#define CALQAPP_HELPERS_H_

// -----------------------------------------------------------------------------

#include <functional>
#include <memory>
#include <string>
#include <utility>

// -----------------------------------------------------------------------------

namespace genie{
namespace quality{
namespace calq {

// -----------------------------------------------------------------------------

std::string currentDateAndTime();
bool fileExists(const std::string& path);
std::string fileBaseName(const std::string& path);
std::string fileNameExtension(const std::string& path);
std::string removeFileNameExtension(const std::string& path);

}  // namespace calq
}  // namespace quality
}  // namespace genie

// -----------------------------------------------------------------------------

#endif  // CALQAPP_HELPERS_H_

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
