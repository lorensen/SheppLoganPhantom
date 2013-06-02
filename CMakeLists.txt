cmake_minimum_required(VERSION 2.8)
 
PROJECT(SheppLoganSurfacePhantom)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(SheppLoganSurfacePhantom MACOSX_BUNDLE
               SheppLoganSurfacePhantom SheppLoganPhantom)
target_link_libraries(SheppLoganSurfacePhantom ${VTK_LIBRARIES})

add_executable(SheppLoganImagePhantom MACOSX_BUNDLE
               SheppLoganImagePhantom SheppLoganPhantom)
target_link_libraries(SheppLoganImagePhantom ${VTK_LIBRARIES})
