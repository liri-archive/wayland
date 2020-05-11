find_package(WaylandClient REQUIRED)
find_package(WaylandServer REQUIRED)
find_package(QtWaylandScanner REQUIRED)

ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../../data/protocols/liri-color-picker.xml"
    BASENAME "liri-color-picker")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../../data/protocols/wayland.xml"
    BASENAME "wayland")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../../data/protocols/wlr-export-dmabuf-unstable-v1.xml"
    BASENAME "wlr-export-dmabuf-unstable-v1")
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
        mockliricolorpicker.cpp
        mockliricolorpicker.h
        mockregistry.cpp
        mockregistry.h
        mockwlrexportdmabufv1.cpp
        mockwlrexportdmabufv1.h
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
        Liri::WaylandServer
        PkgConfig::WaylandClient
        PkgConfig::WaylandServer
)

target_link_libraries(tst_waylandserver PUBLIC -lrt)
