#!/usr/bin/env sh

. ./build.env || exit 1

if ! command -v "$CLANG" >/dev/null; then
    echo "нужна команда $CLANG, установите clang"
fi

if ! command -v "$WLDD" >/dev/null; then
    echo "нужна команда $WLDD, установите lld"
fi

if ! command -v wasm-opt >/dev/null; then
    echo "нужна команда wasm-opt, установите binaryen"
fi

if test "$1" = "APP_MIN"; then
CFLAGS="-D APP_MIN_JS"
rm "$SRC/app.min.js" 2> /dev/null
npx uglifyjs --toplevel --rename "$SRC/app.js" > "$SRC/app.min.js" || exit 1
echo "$1"
fi

"$CLANG" --target="$WARCH" --language=c --std=c23 -Oz --no-standard-libraries \
    $CFLAGS \
    -Wl,--export=init -Wl,--export=input -Wl,--export=process -Wl,--export=render -Wl,--export=__heap_base \
    -Wl,--export=app_js \
    -Wl,--import-undefined \
    -Wl,--strip-all \
    -Wl,--no-entry \
    -o "$BUILD"/app.wasm "$SRC"/app.c || exit 1

chmod -x "$BUILD"/app.wasm

gzip -nm -kf "$BUILD"/app.wasm
base64 -w 0 "$BUILD"/app.wasm > "$BUILD"/app.wasm.base64

echo COMPILED
