find_package(Wayland REQUIRED)
find_package(QtWaylandScanner REQUIRED)

ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${WaylandProtocols_DATADIR}/stable/xdg-shell/xdg-shell.xml"
    BASENAME "xdg-shell")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/liri-shell.xml"
    BASENAME "liri-shell")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/wlr-layer-shell-unstable-v1.xml"
    BASENAME "wlr-layer-shell-unstable-v1")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/wlr-output-management-unstable-v1.xml"
    BASENAME "wlr-output-management-unstable-v1")

set(_generated_files
    "${CMAKE_CURRENT_BINARY_DIR}/wayland-xdg-shell-client-protocol.h"
    "${CMAKE_CURRENT_BINARY_DIR}/wayland-liri-shell-client-protocol.h"
    "${CMAKE_CURRENT_BINARY_DIR}/wayland-wlr-layer-shell-unstable-v1-client-protocol.h"
)
set_source_files_properties(${_generated_files} PROPERTIES SKIP_AUTOMOC ON)

liri_add_module(WaylandClient
    DESCRIPTION
        "Wayland client extensions"
    SOURCES
        lirishell.cpp
        lirishell.h
        lirishell_p.h
        wlrlayershellv1.cpp
        wlrlayershellv1.h
        wlrlayershellv1_p.h
        wlroutputmanagementv1.cpp
        wlroutputmanagementv1.h
        wlroutputmanagementv1_p.h
        ${SOURCES}
    FORWARDING_HEADERS
        LiriShell
        WlrLayerShellV1
        WlrOutputManagementV1
    PRIVATE_HEADERS
        lirishell_p.h
        wlrlayershellv1_p.h
        wlroutputmanagementv1_p.h
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-liri-shell.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-xdg-shell.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-wlr-layer-shell-unstable-v1.h"
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
    PUBLIC_LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Qml
        Qt5::WaylandClient
        Wayland::Client
    LIBRARIES
        Qt5::GuiPrivate
    PKGCONFIG_DEPENDENCIES
        Qt5Core
        Qt5Gui
        Qt5Qml
        Qt5WaylandClient
)
