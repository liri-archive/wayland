find_package(Wayland REQUIRED)
find_package(QtWaylandScanner REQUIRED)

ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../../data/protocols/wayland.xml"
    BASENAME "wayland")

liri_add_test(tst_waylandserver
    SOURCES
        tst_waylandserver.cpp
        mockclient.cpp
        mockclient.h
        mockregistry.cpp
        mockregistry.h
        testcompositor.cpp
        testcompositor.h
        ${SOURCES}
    LIBRARIES
        Qt5::GuiPrivate
        Wayland::Client
        Liri::WaylandServer
)
