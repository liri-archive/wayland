find_package(Wayland REQUIRED)

liri_add_qml_plugin(waylandserver
    MODULE_PATH
        "Liri/WaylandServer"
    VERSION
        "1.0"
    SOURCES
        plugin.cpp
        waylandwlrforeigntoplevelmanagementv1qml.cpp
        waylandwlrforeigntoplevelmanagementv1qml.h
        waylandwlroutputmanagementv1qml.cpp
        waylandwlroutputmanagementv1qml.h
    QML_FILES
        qmldir
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
    PUBLIC_LIBRARIES
        Qt5::Qml
        Qt5::Quick
        Liri::WaylandServer
    LIBRARIES
        Liri::WaylandServerPrivate
)
