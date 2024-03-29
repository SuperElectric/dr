# /usr/share/cmake-2.8/Modules

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

include_directories (
    ${PROJECT_SOURCE_DIR}/include
    ${OpenGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIR}
    ${GTEST_DIR}/include)

#link_directories (${OpenGL_LIBRARY_DIR} ${LIBRARY_OUTPUT_PATH})
#add_definitions(${OpenGL_DEFINITIONS})

add_library (dr SHARED texture.cpp display.cpp mesh.cpp shader.cpp parameterVector.cpp tiny_obj_loader.cpp)
add_library (c_wrapper SHARED c_wrapper.cpp)
add_executable (main main.cpp)
add_executable (tests tests.cpp)


set (librarylist ${OPENGL_LIBRARIES} 
    ${GLEW_LIBRARIES} ${SDL2_LIBRARY} ${GTEST_BOTH_LIBRARIES})
foreach(libname ${librarylist})
    message (STATUS "linking to: " ${libname})
endforeach(libname)

target_link_libraries (dr ${librarylist})
target_link_libraries (main dr c_wrapper)
target_link_libraries (tests dr)
target_link_libraries (c_wrapper dr)
