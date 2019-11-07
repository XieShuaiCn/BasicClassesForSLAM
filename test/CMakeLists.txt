
add_executable(ReadYaml ReadYaml.cpp)
target_link_libraries(ReadYaml ${SLAM_LIBRARY_NAME})

if (SLAM_HAVE_GTEST)
    add_executable(TestYaml TestYaml.cpp)
    target_link_libraries(TestYaml ${SLAM_LIBRARY_NAME} GTest::GTest GTest::Main)
endif ()
