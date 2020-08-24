project("calq")

set(source_files
        calq_coder.cc
        exceptions.cc
        filter_buffer.cc
        genotyper.cc
        haplotyper.cc
        lloyd_max_quantizer.cc
        log.cc
        probability_distribution.cc
        qual_encoder.cc
        quantizer.cc
        sam_pileup.cc
        sam_pileup_deque.cc
        softclip_spreader.cc
        uniform_min_max_quantizer.cc
        uniform_quantizer.cc
        helpers.cc
        exceptions.h
        calq_coder.h
        filter_buffer.h
        genotyper.h
        error_reporter.h
        )

add_library(calq ${source_files})


target_link_libraries(calq PUBLIC genie-core)
target_link_libraries(calq PUBLIC genie-paramqv1)
target_link_libraries(calq PUBLIC genie-qvwriteout)

get_filename_component(THIS_DIR ./ ABSOLUTE)
get_filename_component(TOP_DIR ../../ ABSOLUTE)
target_include_directories(calq PUBLIC "${THIS_DIR}")
target_include_directories(calq PUBLIC "${TOP_DIR}")