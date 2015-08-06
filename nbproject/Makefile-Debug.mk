#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/src/algorithm/abstract_gradient_calculator.o \
	${OBJECTDIR}/src/algorithm/grad_fit_tile_unwrapper.o \
	${OBJECTDIR}/src/algorithm/least_squares_grad_unwrapper.o \
	${OBJECTDIR}/src/algorithm/reliability_calculator_mean_difference.o \
	${OBJECTDIR}/src/algorithm/reliability_calculator_random.o \
	${OBJECTDIR}/src/algorithm/reliability_calculator_variance.o \
	${OBJECTDIR}/src/algorithm/reliability_calculator_variance_second.o \
	${OBJECTDIR}/src/algorithm/second_order_gradient.o \
	${OBJECTDIR}/src/algorithm/simple1d_tile_merger.o \
	${OBJECTDIR}/src/algorithm/simulated_annealing_floodfill_merger.o \
	${OBJECTDIR}/src/algorithm/simulated_annealing_merger.o \
	${OBJECTDIR}/src/algorithm/srncp_tile_merger.o \
	${OBJECTDIR}/src/algorithm/strand_tile_merger.o \
	${OBJECTDIR}/src/algorithm/strand_tile_unwrapper.o \
	${OBJECTDIR}/src/algorithm/wrap.o \
	${OBJECTDIR}/src/analyse/measure_pixel_energy.o \
	${OBJECTDIR}/src/analyse/measure_psnr.o \
	${OBJECTDIR}/src/command_line.o \
	${OBJECTDIR}/src/debug/debug_file_io.o \
	${OBJECTDIR}/src/debug/test_memory_leak.o \
	${OBJECTDIR}/src/image/float_image.o \
	${OBJECTDIR}/src/image/in_out.o \
	${OBJECTDIR}/src/image/row_major_float_image.o \
	${OBJECTDIR}/src/model/model_function.o \
	${OBJECTDIR}/src/model/model_gradient.o \
	${OBJECTDIR}/src/model/monomial_function.o \
	${OBJECTDIR}/src/model/monomial_gradient.o \
	${OBJECTDIR}/src/tiles/tile.o \
	${OBJECTDIR}/src/tiles/tile_dimension_exception.o \
	${OBJECTDIR}/src/tiles/tile_junction.o \
	${OBJECTDIR}/src/tiles/tiled_image.o \
	${OBJECTDIR}/src/tiles/tilegroup.o \
	${OBJECTDIR}/src/tiles/unit_cartesian_coordinate_system.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m64 -static -static-libgcc -static-libstdc++ -O2 -Wno-unused-local-typedefs
CXXFLAGS=-m64 -static -static-libgcc -static-libstdc++ -O2 -Wno-unused-local-typedefs

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=/C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-1_56.a /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-1_56.dll.a /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-d-1_56.a /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-d-1_56.dll.a /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-mt-1_56.a /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-mt-1_56.dll.a /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-mt-d-1_56.a /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-mt-d-1_56.dll.a /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-mt-s-1_56.a /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-mt-sd-1_56.a /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-s-1_56.a /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-sd-1_56.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe: /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-1_56.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe: /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-1_56.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe: /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-d-1_56.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe: /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-d-1_56.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe: /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-mt-1_56.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe: /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-mt-1_56.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe: /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-mt-d-1_56.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe: /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-mt-d-1_56.dll.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe: /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-mt-s-1_56.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe: /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-mt-sd-1_56.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe: /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-s-1_56.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe: /C/Software/boost_1_56_0/stage/lib/libboost_program_options-mgw48-sd-1_56.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/src/algorithm/abstract_gradient_calculator.o: src/algorithm/abstract_gradient_calculator.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/abstract_gradient_calculator.o src/algorithm/abstract_gradient_calculator.cpp

${OBJECTDIR}/src/algorithm/grad_fit_tile_unwrapper.o: src/algorithm/grad_fit_tile_unwrapper.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/grad_fit_tile_unwrapper.o src/algorithm/grad_fit_tile_unwrapper.cpp

${OBJECTDIR}/src/algorithm/least_squares_grad_unwrapper.o: src/algorithm/least_squares_grad_unwrapper.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/least_squares_grad_unwrapper.o src/algorithm/least_squares_grad_unwrapper.cpp

${OBJECTDIR}/src/algorithm/reliability_calculator_mean_difference.o: src/algorithm/reliability_calculator_mean_difference.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/reliability_calculator_mean_difference.o src/algorithm/reliability_calculator_mean_difference.cpp

${OBJECTDIR}/src/algorithm/reliability_calculator_random.o: src/algorithm/reliability_calculator_random.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/reliability_calculator_random.o src/algorithm/reliability_calculator_random.cpp

${OBJECTDIR}/src/algorithm/reliability_calculator_variance.o: src/algorithm/reliability_calculator_variance.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/reliability_calculator_variance.o src/algorithm/reliability_calculator_variance.cpp

${OBJECTDIR}/src/algorithm/reliability_calculator_variance_second.o: src/algorithm/reliability_calculator_variance_second.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/reliability_calculator_variance_second.o src/algorithm/reliability_calculator_variance_second.cpp

${OBJECTDIR}/src/algorithm/second_order_gradient.o: src/algorithm/second_order_gradient.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/second_order_gradient.o src/algorithm/second_order_gradient.cpp

${OBJECTDIR}/src/algorithm/simple1d_tile_merger.o: src/algorithm/simple1d_tile_merger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/simple1d_tile_merger.o src/algorithm/simple1d_tile_merger.cpp

${OBJECTDIR}/src/algorithm/simulated_annealing_floodfill_merger.o: src/algorithm/simulated_annealing_floodfill_merger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/simulated_annealing_floodfill_merger.o src/algorithm/simulated_annealing_floodfill_merger.cpp

${OBJECTDIR}/src/algorithm/simulated_annealing_merger.o: src/algorithm/simulated_annealing_merger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/simulated_annealing_merger.o src/algorithm/simulated_annealing_merger.cpp

${OBJECTDIR}/src/algorithm/srncp_tile_merger.o: src/algorithm/srncp_tile_merger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/srncp_tile_merger.o src/algorithm/srncp_tile_merger.cpp

${OBJECTDIR}/src/algorithm/strand_tile_merger.o: src/algorithm/strand_tile_merger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/strand_tile_merger.o src/algorithm/strand_tile_merger.cpp

${OBJECTDIR}/src/algorithm/strand_tile_unwrapper.o: src/algorithm/strand_tile_unwrapper.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/strand_tile_unwrapper.o src/algorithm/strand_tile_unwrapper.cpp

${OBJECTDIR}/src/algorithm/wrap.o: src/algorithm/wrap.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/algorithm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/algorithm/wrap.o src/algorithm/wrap.cpp

${OBJECTDIR}/src/analyse/measure_pixel_energy.o: src/analyse/measure_pixel_energy.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/analyse
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/analyse/measure_pixel_energy.o src/analyse/measure_pixel_energy.cpp

${OBJECTDIR}/src/analyse/measure_psnr.o: src/analyse/measure_psnr.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/analyse
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/analyse/measure_psnr.o src/analyse/measure_psnr.cpp

${OBJECTDIR}/src/command_line.o: src/command_line.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/command_line.o src/command_line.cpp

${OBJECTDIR}/src/debug/debug_file_io.o: src/debug/debug_file_io.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/debug
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/debug/debug_file_io.o src/debug/debug_file_io.cpp

${OBJECTDIR}/src/debug/test_memory_leak.o: src/debug/test_memory_leak.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/debug
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/debug/test_memory_leak.o src/debug/test_memory_leak.cpp

${OBJECTDIR}/src/image/float_image.o: src/image/float_image.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/image
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/image/float_image.o src/image/float_image.cpp

${OBJECTDIR}/src/image/in_out.o: src/image/in_out.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/image
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/image/in_out.o src/image/in_out.cpp

${OBJECTDIR}/src/image/row_major_float_image.o: src/image/row_major_float_image.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/image
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/image/row_major_float_image.o src/image/row_major_float_image.cpp

${OBJECTDIR}/src/model/model_function.o: src/model/model_function.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/model
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/model/model_function.o src/model/model_function.cpp

${OBJECTDIR}/src/model/model_gradient.o: src/model/model_gradient.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/model
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/model/model_gradient.o src/model/model_gradient.cpp

${OBJECTDIR}/src/model/monomial_function.o: src/model/monomial_function.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/model
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/model/monomial_function.o src/model/monomial_function.cpp

${OBJECTDIR}/src/model/monomial_gradient.o: src/model/monomial_gradient.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/model
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/model/monomial_gradient.o src/model/monomial_gradient.cpp

${OBJECTDIR}/src/tiles/tile.o: src/tiles/tile.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/tiles
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tiles/tile.o src/tiles/tile.cpp

${OBJECTDIR}/src/tiles/tile_dimension_exception.o: src/tiles/tile_dimension_exception.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/tiles
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tiles/tile_dimension_exception.o src/tiles/tile_dimension_exception.cpp

${OBJECTDIR}/src/tiles/tile_junction.o: src/tiles/tile_junction.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/tiles
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tiles/tile_junction.o src/tiles/tile_junction.cpp

${OBJECTDIR}/src/tiles/tiled_image.o: src/tiles/tiled_image.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/tiles
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tiles/tiled_image.o src/tiles/tiled_image.cpp

${OBJECTDIR}/src/tiles/tilegroup.o: src/tiles/tilegroup.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/tiles
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tiles/tilegroup.o src/tiles/tilegroup.cpp

${OBJECTDIR}/src/tiles/unit_cartesian_coordinate_system.o: src/tiles/unit_cartesian_coordinate_system.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/tiles
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I. -Iinclude -I/C/Software/Octave3.6.4_gcc4.6.2/Octave3.6.4_gcc4.6.2/include/octave-3.6.4 -I/C/Software/boost_1_56_0 -I/C/Software/eigen -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tiles/unit_cartesian_coordinate_system.o src/tiles/unit_cartesian_coordinate_system.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/digiholo2d_v2.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
