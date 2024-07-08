set(LIB_RESOURCES 
)
set(LIB_SOURCES
)

qt_add_qml_module(lightManager STATIC
    URI LightManager
    PLUGIN_TARGET lightManager
    VERSION 1.0
    SOURCES ${LIB_SOURCES}
    RESOURCES ${LIB_RESOURCES}

)
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
        target_compile_options(lightManager PUBLIC "/Zc:__cplusplus")
endif()
target_include_directories(lightManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(lightManager PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.lightManager
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(lightManager
    PRIVATE Qt6::Quick Qt6::SerialPort Qt6::Core
)

include(GNUInstallDirs)
install(TARGETS lightManager
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
set(LIB_SOURCE ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")
