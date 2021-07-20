if(NOT TARGET Liri::WaylandClientLayerShell)
    find_package(Liri1WaylandClientLayerShell REQUIRED)
endif()

liri_add_qml_plugin(layershell
    MODULE_PATH
        "Liri/WaylandClient/LayerShell"
    VERSION
        "1.0"
    SOURCES
        plugin.cpp
        layersurfaceqml.cpp
        layersurfaceqml.h
    QML_FILES
        qmldir
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
    PUBLIC_LIBRARIES
        Qt5::Qml
        Qt5::Quick
        Liri::WaylandClientLayerShell
)
