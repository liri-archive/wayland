find_package(Wayland REQUIRED)
find_package(QtWaylandScanner REQUIRED)

ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/wlr-output-management-unstable-v1.xml"
    BASENAME "wlr-output-management-unstable-v1")

liri_add_module(WaylandClient
    DESCRIPTION
        "Wayland client extensions"
    SOURCES
        wlroutputmanagementv1.cpp
        wlroutputmanagementv1.h
        wlroutputmanagementv1_p.h
        ${SOURCES}
    FORWARDING_HEADERS
        WlrOutputManagementV1
    PRIVATE_HEADERS
        wlroutputmanagementv1_p.h
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
