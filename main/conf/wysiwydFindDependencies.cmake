# Copyright: (C) 2014 WYSIWYD Consortium
# Authors: Ugo Pattacini
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

macro(checkandset_dependency package)
    if(${package}_FOUND)
        set(WYSIWYD_HAS_${package} TRUE CACHE BOOL "Package ${package} found" FORCE)
        set(WYSIWYD_USE_${package} TRUE CACHE BOOL "Use package ${package}")
        message(STATUS "found ${package}")
    else()
        set(WYSIWYD_HAS_${package} FALSE CACHE BOOL "" FORCE)
        set(WYSIWYD_USE_${package} FALSE CACHE BOOL "Use package ${package}")
    endif()
    mark_as_advanced(WYSIWYD_HAS_${package})
    
    if(NOT ${package}_FOUND AND WYSIWYD_USE_${package})
        message("Warning: you requested to use the package ${package}, but it is unavailable (or was not found). This might lead to compile errors, we recommend you turn off the WYSIWYD_USE_${package} flag.") 
    endif()
endmacro(checkandset_dependency)

find_package(GSL REQUIRED)
find_package(OpenCV)
find_package(ACE)
find_package(GTK2)
find_package(TUIO)
find_package(IQR)
find_package(KinectSDK)
find_package(PGSQL)
find_package(OpenNI)
find_package(FFTW3)
find_package(OTL)
find_package(RTABMap 0.8.9 QUIET)
find_package(PCL COMPONENTS common io visualization filters QUIET)
find_package(CURL QUIET)
find_package(Qt5 COMPONENTS Core Widgets Gui Quick Qml Multimedia Xml PrintSupport QUIET)
find_package(kinectWrapper QUIET)
find_package(Boost COMPONENTS chrono thread system QUIET)

message(STATUS "I have found the following libraries:")
checkandset_dependency(GSL)
checkandset_dependency(OpenCV)
checkandset_dependency(ACE)
checkandset_dependency(GTK2)
checkandset_dependency(TUIO)
checkandset_dependency(IQR)
checkandset_dependency(KinectSDK)
checkandset_dependency(PGSQL)
checkandset_dependency(OpenNI)
checkandset_dependency(Boost)
checkandset_dependency(FFTW3)
checkandset_dependency(OTL)
checkandset_dependency(RTABMap)
checkandset_dependency(PCL)
checkandset_dependency(CURL)
checkandset_dependency(Qt5)
checkandset_dependency(kinectWrapper)
checkandset_dependency(Boost)
