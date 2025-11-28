# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

idf_component_register(
    SRCS 
        "main.cpp" 
        "sensor_real.cpp"
        "sensor_mock.cpp"
    INCLUDE_DIRS 
        "."
    REQUIRES 
        led_strip
)
