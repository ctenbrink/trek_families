CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(Groups)
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtTest REQUIRED)

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

SET(YAML-CPP_INCLUDE_DIRS /Users/ctenbrink/dev/yaml-cpp/include)
INCLUDE_DIRECTORIES( ${YAML-CPP_INCLUDE_DIRS})

ADD_LIBRARY(yaml-cpp STATIC IMPORTED)
SET_TARGET_PROPERTIES(yaml-cpp PROPERTIES
    IMPORTED_LOCATION /Users/ctenbrink/dev/yaml-cpp/bin/osx/Debug/libyaml-cpp.a)

SET(Groups_SOURCES main.cpp
                   mainwindow.cpp
                   person.cpp
                   document.cpp
                   personmodel.cpp
                   personlistmodel.cpp
                   personlistview.cpp
                   persontablemodel.cpp
                   group.cpp
                   rgbcolor.cpp
                   groupdockwidget.cpp
                   company.cpp
   )
        
SET(Groups_HEADERS 
    mainwindow.h
    person.h
    document.h
    personmodel.h
    personlistmodel.h
    personlistview.h
    persontablemodel.h
    group.h
    rgbcolor.h
    groupdockwidget.h
    company.h
   )

QT4_WRAP_CPP(Groups_HEADERS_MOC ${Groups_HEADERS})

ADD_EXECUTABLE(Groups ${Groups_SOURCES}
                      ${Groups_HEADERS}
                      ${Groups_HEADERS_MOC}
              )

SET(LIBRARIES ${LIBRARIES} ${QT_LIBRARIES} ${YAML-CPP_LIBRARIES} yaml-cpp)

TARGET_LINK_LIBRARIES(Groups ${LIBRARIES})
