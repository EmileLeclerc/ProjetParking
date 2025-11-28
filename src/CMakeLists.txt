# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

idf_component_register(
    SRCS 
        "main.c" 
        "sensor_real.c"
        "sensor_mock.c"
    INCLUDE_DIRS 
        "."
    REQUIRES 
        led_strip
)
