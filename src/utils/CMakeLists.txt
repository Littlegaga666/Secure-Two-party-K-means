SET(CMAKE_CXX_STANDARD 11)
add_library(utils
        data.cpp
        encoder.cpp
        handler.cpp
        easylogging++.cpp
)
find_package (Threads)
target_link_libraries(utils Threads::Threads)
add_library(BIOSECURE::utils ALIAS utils)
