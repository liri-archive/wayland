find_package(Wayland REQUIRED)
find_package(QtWaylandScanner REQUIRED)

liri_add_module(WaylandClient
    DESCRIPTION
        "Wayland client extensions"
    SOURCES
    FORWARDING_HEADERS
    PRIVATE_HEADERS
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
    PUBLIC_LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Qml
        Qt5::WaylandClient
        Wayland::Client
    PKGCONFIG_DEPENDENCIES
        Qt5Core
        Qt5Gui
        Qt5Qml
        Qt5WaylandClient
)
