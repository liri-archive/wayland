find_package(Wayland REQUIRED)
find_package(QtWaylandScanner REQUIRED)

ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/liri-decoration.xml"
    BASENAME "liri-decoration")
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/gtk-shell.xml"
    BASENAME "gtk-shell")
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/server-decoration.xml"
    BASENAME "server-decoration")
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/wlr-output-management-unstable-v1.xml"
    BASENAME "wlr-output-management-unstable-v1")
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/shell-helper.xml"
    BASENAME "shell-helper")

if(IS_ABSOLUTE "${INSTALL_LIBEXECDIR}")
    set(LIBEXECDIR "${INSTALL_LIBEXECDIR}")
else()
    set(LIBEXECDIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIBEXECDIR}")
endif()

liri_add_module(WaylandServer
    DESCRIPTION
        "Wayland server extensions"
    SOURCES
        gtkshell.cpp
        gtkshell.h
        gtkshell_p.h
        kdeserverdecoration.cpp
        kdeserverdecoration.h
        kdeserverdecoration_p.h
        liridecoration.cpp
        liridecoration.h
        liridecoration_p.h
        shellhelper.cpp
        shellhelper.h
        shellhelper_p.h
        waylandwlroutputmanagerv1.cpp
        waylandwlroutputmanagerv1.h
        waylandwlroutputmanagerv1_p.h
        logging.cpp
        logging_p.h
        ${SOURCES}
    FORWARDING_HEADERS
        GtkShell
        KdeServerDecoration
        LiriDecoration
        ShellHelper
        WaylandWlrOutputManagerV1
    PRIVATE_HEADERS
        gtkshell_p.h
        shellhelper_p.h
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-gtk-shell.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-gtk-shell-server-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-server-decoration.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-server-decoration-server-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-liri-decoration.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-liri-decoration-server-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-shell-helper.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-shell-helper-server-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-wlr-output-management-unstable-v1.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-wlr-output-management-unstable-v1-server-protocol.h"
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
        QT_WAYLAND_COMPOSITOR_QUICK
        INSTALL_LIBEXECDIR="${LIBEXECDIR}"
    PUBLIC_LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Quick
        Qt5::WaylandCompositor
        Wayland::Server
    PKGCONFIG_DEPENDENCIES
        Qt5Core
        Qt5Gui
        Qt5WaylandCompositor
)
