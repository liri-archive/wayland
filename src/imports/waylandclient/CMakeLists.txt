find_package(Wayland REQUIRED)

liri_add_qml_plugin(waylandclient
    MODULE_PATH
        "Liri/WaylandClient"
    VERSION
        "1.0"
    SOURCES
        plugin.cpp
    QML_FILES
        qmldir
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
    PUBLIC_LIBRARIES
        Qt5::Qml
        Qt5::Quick
        Liri::WaylandClient
)
