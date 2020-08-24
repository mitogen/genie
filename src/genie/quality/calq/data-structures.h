#ifndef CALQ_DATA_STRUCTURES_H_
#define CALQ_DATA_STRUCTURES_H_

namespace calq {

struct EncodingBlock {
    std::vector<std::string> qualityValues;
};

struct DecodingBlock {
    std::vector<uint8_t> quantizerIndexes;
    std::vector<std::vector<uint8_t>> quantizerStepIndexes;
    std::vector<std::vector<uint8_t>> codebooks;
};

enum struct FilterType { GAUSS, RECT };

enum struct Version { V1, V2 };

struct EncodingOptions {
    /**
     * CALQ V2: Squash activity values between 0.0 and 1.0, to be able to treat them as probabilities
     */
    bool squash = true;

    /**
     * CALQ V2: Filter radius. How big should the filter be? For FilterType::GAUSS this is sigma, for FilterType::RECT
     * it is the radius similar to cutoff.
     */
    size_t filterRadius = 17;

    /**
     * CALQ V2: Filter cutoff. How big should the filter kernel be? Infinite filters like Gauss will be cut off at that
     * point. The value specified here will be the radius of the kernel, such that the kernel size is equal to
     * (filterCutOff * 2) + 1.
     */
    size_t filterCutoff = 50;

    /**
     * CALQ V2: Quality value (without offset) at which a soft clip is considered "high quality"
     */
    uint8_t hqSoftClipThreshold = 29;

    /**
     * CALQ V2: How many bases the activity values are supposed to be spread once the streak limit has been reached
     */
    size_t hqSoftClipPropagation = 50;

    /**
     * CALQ V2: How many high quality softclips there have to be at one position to trigger the propagation
     */
    size_t hqSoftClipStreak = 7;

    /**
     * CALQ V2: Used filter to even out activity value
     */
    FilterType filterType = FilterType::GAUSS;

    uint8_t minNumQuantSteps = 2;
    uint8_t maxNumQuantSteps = 8;
    uint8_t polyploidy = 2;
    uint8_t qualityValueOffset = 33;
    Version version = Version::V1;
};

}  // namespace calq

#endif  // CALQ_DATA_STRUCTURES_H_
