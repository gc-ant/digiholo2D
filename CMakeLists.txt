cmake_minimum_required(VERSION 3.14)
project(digiholo2D)

set(CMAKE_CXX_STANDARD 11)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(include)

add_executable(digiholo2D main.cpp
        src/algorithm/abstract_gradient_calculator.cpp
        src/algorithm/grad_fit_tile_unwrapper.cpp
        src/algorithm/least_squares_grad_unwrapper.cpp
        src/algorithm/reliability_calculator_mean_difference.cpp
        src/algorithm/reliability_calculator_random.cpp
        src/algorithm/reliability_calculator_variance.cpp
        src/algorithm/reliability_calculator_variance_second.cpp
        src/algorithm/second_order_gradient.cpp
        src/algorithm/simple1d_tile_merger.cpp
        src/algorithm/simulated_annealing_floodfill_merger.cpp
        src/algorithm/simulated_annealing_merger.cpp
        src/algorithm/srncp_tile_merger.cpp
        src/algorithm/srncp_tile_merger.cpp
        src/algorithm/strand_tile_merger.cpp
        src/algorithm/strand_tile_unwrapper.cpp
        src/algorithm/wrap.cpp
        src/analyse/measure_pixel_energy.cpp
        src/analyse/measure_psnr.cpp
        src/debug/debug_file_io.cpp
        src/debug/test_memory_leak.cpp
        src/image/float_image.cpp
        src/image/in_out.cpp
        src/image/row_major_float_image.cpp
        src/image/windowfunctions.cpp
        src/model/model_function.cpp
        src/model/model_gradient.cpp
        src/model/monomial_function.cpp
        src/model/monomial_gradient.cpp
        src/tiles/tile.cpp
        src/tiles/tile_dimension_exception.cpp
        src/tiles/tile_junction.cpp
        src/tiles/tiled_image.cpp
        src/tiles/tilegroup.cpp
        src/tiles/unit_cartesian_coordinate_system.cpp include/fringe/windowfunctions.h
        src/command_line.cpp)

target_link_libraries(digiholo2D ${CONAN_LIBS})

if (UNIX)
    #boost stacktrace needs this on POSIX systems, see https://github.com/boostorg/stacktrace/issues/80
    target_link_libraries(digiholo2D dl)
endif ()

if(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(digiholo2D PUBLIC "$<$<CONFIG:RELEASE>:-O3>;-march=native;-fopenmp")
    # in case you want to enable BLAS/LAPACK integration in Eigen as well as OpenMP, use the following
    # (but be sure to have the blas and lapacke (and open mp) libraries installed on the system):
    # target_compile_definitions(digiholo2D PUBLIC EIGEN_USE_BLAS EIGEN_USE_LAPACKE)
    # target_link_libraries(digiholo2D lapacke blas gomp)
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
endif ()
