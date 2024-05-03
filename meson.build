project('cpp-nostr',
        'cpp',
        default_options : [
                'warning_level=3',
                'cpp_std=gnu++17'
        ])

src = ['main.cpp']

cpp = meson.get_compiler('cpp')

pthread = dependency('threads')
yyjson = dependency('yyjson')

cmake = import('cmake')
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
    '-DLIBBECH32_BUILD_TESTS=OFF'
  ]
)
libbech32_dep = libbech32_subproject.dependency('bech32')

rx_nostr_cpp_proj = subproject('rx-nostr-cpp')
rx_nostr_cpp_dep = rx_nostr_cpp_proj.get_variable('rx_nostr_cpp_dep')

executable('main', src, cpp_args:[
        ],link_args:[
        '-fuse-ld=lld'
        ],dependencies:[
        pthread,yyjson,secp256k1_dep,libbech32_dep,libhv_dep,rx_nostr_cpp_dep
        ])
