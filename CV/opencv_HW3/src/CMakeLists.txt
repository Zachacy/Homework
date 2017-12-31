ADD_EXECUTABLE( main main.cpp )
find_package( OpenCV  REQUIRED )
target_link_libraries( main ${OpenCV_LIBS} )
