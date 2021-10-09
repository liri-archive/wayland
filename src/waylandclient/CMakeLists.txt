find_package(QtWaylandScanner REQUIRED)

ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/liri-color-picker.xml"
    BASENAME "liri-color-picker")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/liri-lockscreen-unstable-v1.xml"
    BASENAME "liri-lockscreen-unstable-v1")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/liri-shell.xml"
    BASENAME "liri-shell")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/wlr-export-dmabuf-unstable-v1.xml"
    BASENAME "wlr-export-dmabuf-unstable-v1")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/wlr-foreign-toplevel-management-unstable-v1.xml"
    BASENAME "wlr-foreign-toplevel-management-unstable-v1")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/wlr-output-management-unstable-v1.xml"
    BASENAME "wlr-output-management-unstable-v1")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/wlr-screencopy-unstable-v1.xml"
    BASENAME "wlr-screencopy-unstable-v1")

set(_generated_c_protocol_files
    "${CMAKE_CURRENT_BINARY_DIR}/wayland-liri-color-picker-client-protocol.h"
    "${CMAKE_CURRENT_BINARY_DIR}/wayland-liri-lockscreen-unstable-v1-client-protocol.h"
    "${CMAKE_CURRENT_BINARY_DIR}/wayland-liri-shell-client-protocol.h"
    "${CMAKE_CURRENT_BINARY_DIR}/wayland-wlr-export-dmabuf-unstable-v1-client-protocol.h"
    "${CMAKE_CURRENT_BINARY_DIR}/wayland-wlr-output-management-unstable-v1-client-protocol.h"
    "${CMAKE_CURRENT_BINARY_DIR}/wayland-wlr-screencopy-unstable-v1-client-protocol.h"
)
set(_generated_cpp_protocol_files
    "${CMAKE_CURRENT_BINARY_DIR}/qwayland-liri-color-picker.h"
    "${CMAKE_CURRENT_BINARY_DIR}/qwayland-liri-lockscreen-unstable-v1.h"
    "${CMAKE_CURRENT_BINARY_DIR}/qwayland-liri-shell.h"
    "${CMAKE_CURRENT_BINARY_DIR}/qwayland-wlr-export-dmabuf-unstable-v1.h"
    "${CMAKE_CURRENT_BINARY_DIR}/qwayland-wlr-foreign-toplevel-management-unstable-v1.h"
    "${CMAKE_CURRENT_BINARY_DIR}/qwayland-wlr-screencopy-unstable-v1.h"
)
set_source_files_properties(${_generated_c_protocol_files} PROPERTIES SKIP_AUTOMOC ON)

liri_add_module(WaylandClient
    DESCRIPTION
        "Wayland client extensions"
    SOURCES
        liricolorpicker.cpp
        liricolorpicker.h
        liricolorpicker_p.h
        lirilockscreenv1.cpp lirilockscreenv1_p.h lirilockscreenv1.h
        lirimodal.cpp
        lirimodal.h
        lirimodal_p.h
        lirishell.cpp
        lirishell.h
        lirishell_p.h
        logging.cpp
        logging_p.h
        utils_p.h
        wlrexportdmabufv1.cpp
        wlrexportdmabufv1.h
        wlrexportdmabufv1_p.h
        wlrforeigntoplevelmanagementv1.cpp
        wlrforeigntoplevelmanagementv1.h
        wlrforeigntoplevelmanagementv1_p.h
        wlrlayersurfacev1.cpp
        wlrlayersurfacev1.h
        wlrlayersurfacev1_p.h
        wlroutputmanagementv1.cpp
        wlroutputmanagementv1.h
        wlroutputmanagementv1_p.h
        wlrscreencopyv1.cpp
        wlrscreencopyv1.h
        wlrscreencopyv1_p.h
        ${SOURCES}
    PRIVATE_HEADERS
        liricolorpicker_p.h
        lirilockscreenv1_p.h
        lirimodal_p.h
        lirishell_p.h
        wlrexportdmabufv1_p.h
        wlrforeigntoplevelmanagementv1_p.h
        wlrlayersurfacev1_p.h
        wlroutputmanagementv1_p.h
        wlrscreencopyv1_p.h
        ${_generated_c_protocol_files}
        ${_generated_cpp_protocol_files}
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
        wayland-client
)

liri_finalize_module(WaylandClient)
