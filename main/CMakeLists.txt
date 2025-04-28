# Link main file to executable namee
add_executable(microgrid main_microgrid.cpp)



# Add required libraries and include directories
target_include_directories(microgrid PRIVATE "." "include" $ENV{CADMIUM})


# set c++ standard
target_compile_options(microgrid PUBLIC -std=gnu++2b)
