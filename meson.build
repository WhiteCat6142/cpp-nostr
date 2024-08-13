project('cpp-nostr',
        'cpp',
        default_options : {
                'warning_level':'2',
                'cpp_std':'gnu++20'
        })

src = ['main.cpp']

cpp = meson.get_compiler('cpp')

pthread = dependency('threads')

openssl = dependency('openssl')

cmake = import('cmake')

yyjson_subproject = cmake.subproject(
  'yyjson',
  cmake_options: [
  ]
)
yyjson_dep = yyjson_subproject.dependency('yyjson')

libhv_subproject = cmake.subproject(
  'libhv',
  cmake_options: [
    '-DWITH_OPENSSL=ON'
  ]
)
libhv_dep = libhv_subproject.dependency('hv')

secp256k1_subproject = cmake.subproject(
  'secp256k1',
  cmake_options: [
    '-DSECP256K1_ENABLE_MODULE_SCHNORRSIG=ON', '-DBUILD_SHARED_LIBS=OFF'
  ]
)
secp256k1_dep = secp256k1_subproject.dependency('secp256k1')

libbech32_subproject = cmake.subproject(
  'libbech32',
  cmake_options: [
  ]
)
libbech32_dep = libbech32_subproject.dependency('bech32')


fmt_subproject = cmake.subproject(
  'fmt',
  cmake_options: [
  ]
)
fmt_dep = fmt_subproject.dependency('fmt')

nameof_subproject = cmake.subproject(
  'nameof',
  cmake_options: [
    '-DCMAKE_BUILD_TYPE=Release', '-DNAMEOF_OPT_INSTALL=ON'
  ]
)
nameof_dep = nameof_subproject.dependency('nameof')

validator_cmake_opts = cmake.subproject_options()
validator_cmake_opts.add_cmake_defines({'fmt_DIR': (meson.project_build_root() / 'subprojects' / 'fmt' / '__CMake_build')})
validator_cmake_opts.add_cmake_defines({'nameof_DIR': (meson.project_build_root() / 'subprojects' / 'nameof' / '__CMake_build')})
validator_cmake_opts.add_cmake_defines({'CMAKE_PREFIX_PATH': meson.project_build_root() / 'subprojects' / 'yyjson' / '__CMake_build'})
validator_cmake_opts.add_cmake_defines({'CPPYYJSON_BUILD_TEST':'OFF'})

cpp_yyjson_subproject = cmake.subproject(
  'cpp-yyjson',
  options: validator_cmake_opts
)
cpp_yyjson_dep = cpp_yyjson_subproject.dependency('cpp_yyjson')

rx_nostr_cpp_proj = subproject('rx-nostr-cpp')
rx_nostr_cpp_dep = rx_nostr_cpp_proj.get_variable('rx_nostr_cpp_dep')

executable('main', src, cpp_args:[
        '-pedantic'
        ],link_args:[
        '-fuse-ld=lld'
        ],dependencies:[
        pthread,openssl,fmt_dep,nameof_dep,yyjson_dep,cpp_yyjson_dep,secp256k1_dep,libbech32_dep,libhv_dep,rx_nostr_cpp_dep
        ])
