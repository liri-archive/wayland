find_package(Wayland REQUIRED)
find_package(QtWaylandScanner REQUIRED)

ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../../data/protocols/wayland.xml"
    BASENAME "wayland")
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../../data/protocols/wlr-output-management-unstable-v1.xml"
    BASENAME "wlr-output-management-unstable-v1")

liri_add_test(tst_waylandserver
    SOURCES
        tst_waylandserver.cpp
        mockclient.cpp
        mockclient.h
        mockregistry.cpp
        mockregistry.h
        mockwlroutputmanagementv1.cpp
        mockwlroutputmanagementv1.h
        testcompositor.cpp
        testcompositor.h
        ${SOURCES}
    LIBRARIES
        Qt5::GuiPrivate
        Wayland::Client
        Liri::WaylandServer
)
