project("genie-basecoder")

set(source_files
        decoder.cc
        encoder.cc)

add_library(genie-basecoder ${source_files})

target_link_libraries(genie-basecoder PUBLIC genie-core)