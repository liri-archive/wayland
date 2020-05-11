find_package(PkgConfig)
pkg_check_modules(WaylandServer "wayland-server >= 1.15" IMPORTED_TARGET)
