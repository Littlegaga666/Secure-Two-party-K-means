find_package(OpenSSL REQUIRED)
add_executable(test_utils test_utils.cpp)
target_link_libraries(test_utils BIOSECURE::algorithms)

add_executable(test_rvp alg_rvp.cpp rvp_rec.cpp rvp_snd.cpp)
target_link_libraries(test_rvp
    BIOSECURE::algorithms
    BIOSECURE::paillier)
