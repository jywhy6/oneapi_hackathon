cmake_minimum_required(VERSION 3.13)

project(oneMKL LANGUAGES C)
enable_testing()
find_package(MKL CONFIG REQUIRED)
find_package(FFTW3f CONFIG REQUIRED)
add_compile_options(/wd4996)

# target 0: use oneMKL API to generate random numbers
add_executable(random_number_generator random_number_generator.c)
target_compile_options(
  random_number_generator
  PRIVATE $<TARGET_PROPERTY:MKL::MKL,INTERFACE_COMPILE_OPTIONS>)
target_include_directories(
  random_number_generator
  PRIVATE $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>
          "${MKL_INCLUDE}/fftw")
target_link_libraries(random_number_generator PRIVATE $<LINK_ONLY:MKL::MKL>)
add_test(NAME test_random_number_generator COMMAND random_number_generator)
if(MKL_ENV)
  set_tests_properties(test_random_number_generator PROPERTIES ENVIRONMENT
                                                               "${MKL_ENV}")
endif()

# target 1: use oneMKL provided FFTW3 API
add_executable(fftw3_onemkl fftw_plan_dft_r2c_2d.c)
target_compile_definitions(fftw3_onemkl PRIVATE USE_ONEMKL=1)
target_compile_options(
  fftw3_onemkl PRIVATE $<TARGET_PROPERTY:MKL::MKL,INTERFACE_COMPILE_OPTIONS>)
target_include_directories(
  fftw3_onemkl PRIVATE $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>
                       "${MKL_INCLUDE}/fftw")
target_link_libraries(fftw3_onemkl PRIVATE $<LINK_ONLY:MKL::MKL>)
add_test(NAME test_fftw3_onemkl COMMAND fftw3_onemkl)
if(MKL_ENV)
  set_tests_properties(test_fftw3_onemkl PROPERTIES ENVIRONMENT "${MKL_ENV}")
endif()

# target 2: use original FFTW3 API
add_executable(fftw3_original fftw_plan_dft_r2c_2d.c)
target_compile_definitions(fftw3_original PRIVATE USE_ONEMKL=0)
target_include_directories(fftw3_original PRIVATE ${FFTW3})
target_link_libraries(fftw3_original PRIVATE FFTW3::fftw3f)
add_test(NAME test_fftw3_original COMMAND fftw3_original)

# target 3: compare the results
add_executable(compare compare.c)
add_test(NAME test_compare COMMAND compare)
