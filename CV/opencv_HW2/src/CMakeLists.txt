ADD_EXECUTABLE( main main.cpp )
find_package( OpenCV 3.0 REQUIRED )
target_link_libraries( main ${OpenCV_LIBS} )
