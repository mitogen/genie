#ifndef GENIE_RETURN_STRUCTURES_H
#define GENIE_RETURN_STRUCTURES_H

#include <map>
#include <string>
#include <vector>

class generated_aus_ref {
    std::vector<std::map<uint8_t, std::map<uint8_t, std::string>>> refAus;
    std::vector<uint64_t> refStart;
    std::vector<uint64_t> refEnd;

   public:
    generated_aus_ref(const std::vector<std::map<uint8_t, std::map<uint8_t, std::string>>> &refAus,
                      const std::vector<uint64_t> &refStart, const std::vector<uint64_t> &refEnd);

    // TODO Jan check these
    virtual ~generated_aus_ref() = default;
    generated_aus_ref(const generated_aus_ref&) = delete;
    generated_aus_ref& operator=(const generated_aus_ref&) = delete;
    generated_aus_ref(generated_aus_ref&&) = default;
    generated_aus_ref& operator=(generated_aus_ref&&) = delete;

    const std::vector<std::map<uint8_t, std::map<uint8_t, std::string>>> &getRefAus() const;

    const std::vector<uint64_t> &getRefStart() const;

    const std::vector<uint64_t> &getRefEnd() const;
};

class generated_aus {
    std::vector<std::map<uint8_t, std::map<uint8_t, std::string>>> encodedFastqAus;

   public:
    generated_aus(const std::vector<std::map<uint8_t, std::map<uint8_t, std::string>>> &encodedFastqAus);

    // TODO Jan check these
    virtual ~generated_aus() = default;
    generated_aus(const generated_aus&) = delete;
    generated_aus& operator=(const generated_aus&) = delete;
    generated_aus(generated_aus&&) = default;
    generated_aus& operator=(generated_aus&&) = delete;

    const std::vector<std::map<uint8_t, std::map<uint8_t, std::string>>> &getEncodedFastqAus() const;
};

#endif  // GENIE_RETURN_STRUCTURES_H
