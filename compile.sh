#!/usr/bin/env sh

. ./build.env || exit 1

if ! command -v clang >/dev/null; then
    echo "нужна команда clang, установите clang"
fi

if ! command -v wasm-opt >/dev/null; then
    echo "нужна команда wasm-opt, установите binaryen"
fi

clang --target="$WARCH" --std=c23 -Oz --no-standard-libraries \
    -Wl,--export=init -Wl,--export=input -Wl,--export=process -Wl,--export=render -Wl,--export=__heap_base -Wl,--no-entry -Wl,--strip-all \
    -o "$BUILD"/app.wasm "$SRC"/app.c || exit 1

chmod -x "$BUILD"/app.wasm

gzip -nm -kf "$BUILD"/app.wasm
base64 -w 0 "$BUILD"/app.wasm > "$BUILD"/app.wasm.base64

echo OK
