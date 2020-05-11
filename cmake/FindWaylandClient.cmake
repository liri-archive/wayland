find_package(PkgConfig)
pkg_check_modules(WaylandClient "wayland-client >= 1.15" IMPORTED_TARGET)
