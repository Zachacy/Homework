find_package(FLTK REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${FLTK_INCLUDE_DIRS})

ADD_LIBRARY( SMFLoader SMFLoader.cpp )

add_executable(FLTK_HW3 main.cpp)
target_link_libraries(FLTK_HW3 SMFLoader ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${FLTK_LIBRARIES})
