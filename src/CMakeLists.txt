add_library(afop_translator_backend_Core STATIC
        network/Server.cpp
        ../include/Server.h
        fileutils/LocPackFile.cpp
        ../include/LocPackFile.h
        utils/LocaleLine.cpp
        ../include/LocaleLine.h
        fileutils/LocPackBinFile.cpp
        ../include/LocPackBinFile.h)

target_include_directories(afop_translator_backend_Core PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(afop_translator_backend main.cpp
        ../include/libraries/rapidcsv.h
)

target_link_libraries(afop_translator_backend PRIVATE afop_translator_backend_Core)