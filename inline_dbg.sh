#!/usr/bin/env bash

. ./build.env || exit 1

OP="Uint8"
if test ! -z "$1"; then
    OP="$1"
fi

echo "$OP"

case "$OP" in
"Uint8")
    OUT_HTML="$BUILD"/app_A_dbg.html
    node tools/stringify.js "$BUILD"/app.wasm >"$BUILD"/main0.js || exit 1
    ;;
"GZipArray")
    OUT_HTML="$BUILD"/app_G_dbg.html
    node tools/stringify.js "$BUILD"/app.wasm.gz >"$BUILD"/main0.js || exit 1
    >"$BUILD"/main.js
    ;;
"Base64")
    OUT_HTML="$BUILD"/app_B_dbg.html
    node tools/stringify.js "$BUILD"/app.wasm.base64 >"$BUILD"/main0.js || exit 1
    >"$BUILD"/main.js
    ;;
*)
    echo "??? $OP"
    exit 1
    ;;
esac

echo  "$OUT_HTML"
# приписываем wasm к js:
head -n 1 "$BUILD"/main0.js >"$BUILD"/main.js || exit 1
tail -n+2 "$SRC"/boot_tpl.js >>"$BUILD"/main.js || exit 1
tail -n 1 "$BUILD"/main0.js >>"$BUILD"/main.js || exit 1
rm "$BUILD"/main0.js

# обновляем _dbg:
sed -e 's/main\.z\.js/main.js/' "$SRC"/html_tpl.html > "$BUILD"/boot_tpl.html || exit 1
node tools/inline.js "$BUILD"/boot_tpl.html "$BUILD" > "$OUT_HTML" || exit 1
rm "$BUILD"/boot_tpl.html

echo DBG
