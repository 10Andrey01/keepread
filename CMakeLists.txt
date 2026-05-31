cmake_minimum_required(VERSION 3.16)

project(keepread LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network WebSockets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network WebSockets Sql)


add_executable(keepread
  main.cpp
  wserver.h wserver.cpp
)
target_link_libraries(keepread Qt${QT_VERSION_MAJOR}::Core
    Qt6::Network
    Qt6::WebSockets
    Qt6::Sql
    )

include(GNUInstallDirs)
install(TARGETS keepread
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
