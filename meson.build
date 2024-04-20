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

rx_nostr_cpp_proj = subproject('rx-nostr-cpp')
rx_nostr_cpp_dep = rx_nostr_cpp_proj.get_variable('rx_nostr_cpp_dep')

executable('main', src, cpp_args:[
        ],link_args:[
        '-fuse-ld=lld'
        ],dependencies:[
        pthread,yyjson,libhv_dep,rx_nostr_cpp_dep
        ])
