project(MIRACL DESCRIPTION "Multiprecision Integer and Rational Arithmetic Cryptographic Library"
               LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.10)

option(MIRACL_BUILD_EXE "Build executables" OFF)


# for linux 64bit
execute_process(
    COMMAND cmake -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/include/mirdef.h64" "${CMAKE_CURRENT_SOURCE_DIR}/include/mirdef.h"
    COMMAND cmake -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/source/mrmuldv.g64" "${CMAKE_CURRENT_SOURCE_DIR}/source/mrmuldv.c"
)


add_library(miracl
    source/mraes.c source/mralloc.c source/mrarth0.c source/mrarth1.c
    source/mrarth2.c source/mrarth3.c source/mrbits.c source/mrbrick.c
    source/mrbuild.c source/mrcore.c source/mrcrt.c source/mrcurve.c
    source/mrdouble.c source/mrebrick.c source/mrec2m.c source/mrecn2.c
    source/mrfast.c source/mrflash.c source/mrflsh1.c source/mrflsh2.c
    source/mrflsh3.c source/mrflsh4.c source/mrfpe.c source/mrfrnd.c
    source/mrgcd.c source/mrgcm.c source/mrgf2m.c source/mrio1.c source/mrio2.c
    source/mrjack.c source/mrlucas.c source/mrmonty.c source/mrpi.c
    source/mrpower.c source/mrprime.c source/mrrand.c source/mrround.c
    source/mrscrt.c source/mrsha3.c source/mrshs256.c source/mrshs512.c
    source/mrshs.c source/mrsmall.c source/mrsroot.c source/mrstrong.c
    source/mrxgcd.c source/mrzzn2b.c source/mrzzn2.c source/mrzzn3.c
    source/mrzzn4.c source/mrmuldv.c
    source/big.cpp source/crt.cpp source/ec2.cpp source/ecn.cpp
    source/flash.cpp source/curve/poly.cpp source/curve/polymod.cpp
    source/zzn.cpp
)
add_library(MIRACL::miracl ALIAS miracl)


target_include_directories(miracl PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


install(TARGETS miracl
        EXPORT "${PROJECT_NAME}Targets"
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        INCLUDES DESTINATION include
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
)
export(TARGETS miracl NAMESPACE "${PROJECT_NAME}::" FILE "${PROJECT_NAME}Targets.cmake")
install(EXPORT "${PROJECT_NAME}Targets"
        NAMESPACE "${PROJECT_NAME}::"
        DESTINATION "lib/cmake/${PROJECT_NAME}"
)

include(CMakePackageConfigHelpers)

configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    DESTINATION "lib/cmake/${PROJECT_NAME}"
)


if(MIRACL_BUILD_EXE)
    foreach(filename source/bmark.c
                     source/fact.c
                     source/mersenne.cpp
                     source/brent.cpp
                     source/sample.cpp
                     source/ecsgen.cpp
                     source/ecsign.cpp
                     source/ecsver.cpp
                     source/pk-demo.cpp
                     source/curve/schoof.cpp)
        get_filename_component(exe_name "${filename}" NAME_WE)
        add_executable("${exe_name}" "${filename}")
        target_include_directories("${exe_name}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_link_libraries("${exe_name}" PRIVATE miracl)
    endforeach()

    install(TARGETS
        bmark fact mersenne brent sample ecsgen ecsign ecsver pk-demo schoof
        DESTINATION bin
    )
endif(MIRACL_BUILD_EXE)


