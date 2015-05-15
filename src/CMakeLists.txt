
file(GLOB_RECURSE AncientNMd_SOURCES "*.cpp")
file(GLOB_RECURSE AncientNMd_HEADERS "*.h")

find_package (Threads)
add_executable (ancientnmd ${AncientNMd_SOURCES})
target_link_libraries (ancientnmd ${CMAKE_THREAD_LIBS_INIT})
target_compile_features(ancientnmd PRIVATE cxx_range_for)