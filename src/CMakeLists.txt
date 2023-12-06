cmake_minimum_required(VERSION 3.16)
project(Scribe VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

qt_add_executable(Scribe WIN32 MACOSX_BUNDLE
    documentmetrics.h
    editor.cpp editor.h
    finddialog.cpp finddialog.h
    gotodialog.cpp gotodialog.h
    highlighters/chighlighter.cpp highlighters/chighlighter.h
    highlighters/cpphighlighter.cpp highlighters/cpphighlighter.h
    highlighters/highlighter.cpp highlighters/highlighter.h
    highlighters/javahighlighter.cpp highlighters/javahighlighter.h
    highlighters/pythonhighlighter.cpp highlighters/pythonhighlighter.h
    language.cpp language.h
    linenumberarea.h
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    metricreporter.cpp metricreporter.h
    searchhistory.cpp searchhistory.h
    settings.cpp settings.h
    tabbededitor.cpp tabbededitor.h
    utilityfunctions.cpp utilityfunctions.h
)
target_compile_definitions(Scribe PRIVATE
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(Scribe PRIVATE
    Qt::Core
    Qt::Gui
    Qt::PrintSupport
)


# Resources:
set(resources_resource_files
    "res/icons/compfile-256.png"
    "res/icons/copy.bmp"
    "res/icons/cut.bmp"
    "res/icons/folder.bmp"
    "res/icons/new.bmp"
    "res/icons/paste.bmp"
    "res/icons/printer.bmp"
    "res/icons/redo.bmp"
    "res/icons/save-as.bmp"
    "res/icons/save.bmp"
    "res/icons/search.bmp"
    "res/icons/undo.bmp"
    "res/icons/upload.png"
)

qt_add_resources(Scribe "resources"
    PREFIX
        "/icons"
    FILES
        ${resources_resource_files}
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(Scribe PRIVATE
        Qt::Widgets
    )
endif()

install(TARGETS Scribe
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
