vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO yosh-matsuda/cpp-yyjson
    REF 506d59a8627ae58de92d9934113245ba83872a4c
    SHA512 d4ab75dbf490e087e9a6b5edb12816a28848e7d6661dcd69944927d8a264d9d229d3b690138277a22e496fbc453bb9aa657bbd1d9835e2f832d3eac65500f112
    HEAD_REF main
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DCPPYYJSON_BUILD_TEST=OFF
)

vcpkg_cmake_install()

vcpkg_fixup_pkgconfig()
vcpkg_cmake_config_fixup(
    PACKAGE_NAME cpp_yyjson
    CONFIG_PATH lib/cmake/cpp_yyjson
)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")