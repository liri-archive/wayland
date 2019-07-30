find_package(Wayland REQUIRED)

liri_add_qml_plugin(waylandserver
    MODULE_PATH
        "Liri/WaylandServer"
    VERSION
        "1.0"
    SOURCES
        plugin.cpp
    QML_FILES
        qmldir
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
        QT_WAYLAND_COMPOSITOR_QUICK
    PUBLIC_LIBRARIES
        Qt5::Qml
        Qt5::Quick
        Liri::WaylandServer
        Liri::WaylandServerPrivate
    STATIC
)
