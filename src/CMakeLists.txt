find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

configure_file(config.h.in config.h)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(usrrerrbar
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(usrrerrbar
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(usrrerrbar PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(usrrerrbar PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER net.rrerr.usrrerrbar
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(usrrerrbar)
endif()
