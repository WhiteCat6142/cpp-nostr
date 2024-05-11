# cpp-nostr

### Build & Installation
Require ninja, meson, cmake and lld to build this software.
Also this depends on yyjson, rx-nostr-cpp, libhv, libbech32, secp256k1 which are automatically installed by meson.

To compile, execute the following commands.
```sh
meson setup ./build
ninja -v -C ./build
```

If you are using VSCode, you could build with ctrl-Shift-B after an initial setup.

### Licence
MIT License

See the LICENSE file for details.

### Acknowledge

This project is inspired by projects below.

* [https://github.com/Hakkadaikon/rx-nostr-cpp](https://github.com/Hakkadaikon/rx-nostr-cpp)
* [https://github.com/mattn/nostr-cxx-bot](https://github.com/mattn/nostr-cxx-bot)

I would like to thank to Mr.Hakkadaikon and Mr.mattn for useful codes.