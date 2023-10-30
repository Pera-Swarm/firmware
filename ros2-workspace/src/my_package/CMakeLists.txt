cmake_minimum_required(VERSION 3.5)
project(my_package)

# Set the C++ standard for your package
set(CMAKE_CXX_STANDARD 17)

# Find necessary dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

# Add your C++ source files
add_executable(my_node src/my_node.cpp)

# Link your C++ node with the required dependencies
ament_target_dependencies(my_node rclcpp)

# Install your executable
install(TARGETS
    my_node
    DESTINATION lib/${PROJECT_NAME}
)

ament_package()
