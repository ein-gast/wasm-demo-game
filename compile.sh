#!/usr/bin/env sh

. ./build.env || exit 1

clang --target="$WARCH" --std=c23 -Oz --no-standard-libraries \
    -Wl,--export=init -Wl,--export=input -Wl,--export=process -Wl,--export=render -Wl,--export=__heap_base -Wl,--no-entry -Wl,--strip-all \
    -o "$BUILD"/app.wasm "$SRC"/app.c || exit 1
# ~ -Wl,--export-all

chmod -x "$BUILD"/app.wasm
gzip -kf "$BUILD"/app.wasm

echo OK
