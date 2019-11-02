find_package(QtWaylandScanner REQUIRED)

ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../../data/protocols/wayland.xml"
    BASENAME "wayland")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../../data/protocols/wlr-foreign-toplevel-management-unstable-v1.xml"
    BASENAME "wlr-foreign-toplevel-management-unstable-v1")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../../data/protocols/wlr-layer-shell-unstable-v1.xml"
    BASENAME "wlr-layer-shell-unstable-v1")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../../data/protocols/wlr-output-management-unstable-v1.xml"
    BASENAME "wlr-output-management-unstable-v1")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../../data/protocols/wlr-screencopy-unstable-v1.xml"
    BASENAME "wlr-screencopy-unstable-v1")

liri_add_test(tst_waylandserver
    SOURCES
        tst_waylandserver.cpp
        mockclient.cpp
        mockclient.h
        mockregistry.cpp
        mockregistry.h
        mockwlrforeigntoplevelmanagementv1.cpp
        mockwlrforeigntoplevelmanagementv1.h
        mockwlrlayershellv1.cpp
        mockwlrlayershellv1.h
        mockwlroutputmanagementv1.cpp
        mockwlroutputmanagementv1.h
        mockwlrscreencopyv1.cpp
        mockwlrscreencopyv1.h
        testcompositor.cpp
        testcompositor.h
        ${SOURCES}
    LIBRARIES
        Qt5::GuiPrivate
        Wayland::Client
        Wayland::Server
        Liri::WaylandServer
)

target_link_options(tst_waylandserver PRIVATE -lrt)
