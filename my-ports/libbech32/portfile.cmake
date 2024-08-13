vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO dcdpr/libbech32
    REF d77814708d5afe8d0c7bd42eb9ea7c85916b3ac2
    SHA512 6780c407eb4247bd8e8d106f2104e7c3c511804d29330f8d9bec5c849c701082e8268716e90d0b162d2c591a2700abcaf62f4bb3fdff43edbdac2ae35b952405
    HEAD_REF master
)


vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    FEATURES
    examples         LIBBECH32_BUILD_EXAMPLES
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        ${FEATURE_OPTIONS}
)
vcpkg_cmake_install()

vcpkg_fixup_pkgconfig()

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")

vcpkg_copy_pdbs()