add_library(algorithms
        alg_or.cpp
        algorithm_handler.cpp
)
add_library(BIOSECURE::algorithms ALIAS algorithms)
target_link_libraries(algorithms ABY::aby ENCRYPTO_utils::encrypto_utils BIOSECURE::utils)