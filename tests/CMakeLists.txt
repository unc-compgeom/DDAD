enable_testing()
# Tell CMake to run moc when necessary:

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Test)

add_executable(tst_Geometry tst_Geometry.cpp)
add_test(tst_Geometry tst_Geometry)

target_link_libraries(tst_Geometry
Qt5::Test
geometry
mpir
mpirxx
)
