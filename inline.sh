#!/usr/bin/env sh

. ./build.env || exit 1

OP="Uint8"
if test ! -z "$1"; then
    OP="$1"
fi

echo "$OP"

./compile.sh APP_MIN

case "$OP" in
"Uint8")
    OUT_HTML="$BUILD"/app_A.html
    node tools/stringify.js "$BUILD"/app.wasm >"$BUILD"/main0.js || exit 1
    ;;
"GZipArray")
    OUT_HTML="$BUILD"/app_G.html
    node tools/stringify.js "$BUILD"/app.wasm.gz >"$BUILD"/main0.js || exit 1
    :>"$BUILD"/main.js
    ;;
"Base64")
    OUT_HTML="$BUILD"/app_B.html
    node tools/stringify.js "$BUILD"/app.wasm.base64 >"$BUILD"/main0.js || exit 1
    :>"$BUILD"/main.js
    ;;
*)
    echo "??? $OP"
    exit 1
    ;;
esac

# приписываем wasm к js:
head -n 1 "$BUILD"/main0.js >"$BUILD"/main.js || exit 1
tail -n+2 "$SRC"/boot_tpl.js >>"$BUILD"/main.js || exit 1
tail -n 1 "$BUILD"/main0.js >>"$BUILD"/main.js || exit 1
rm "$BUILD"/main0.js

# минимизируем js:
npx uglifyjs --toplevel --keep-fargs --rename "$BUILD"/main.js  > "$BUILD"/main.u.js  || exit 1
npx regpack --reassignVars 0 "$BUILD"/main.u.js | sed -e 's/^stats:.*$//g' > "$BUILD"/main.z.js || exit 1

# встраиваем js в html:
echo "$OUT_HTML"
node tools/inline.js "$SRC"/html_tpl.html "$BUILD" | tr -d '\r' | tr -d '\n' > "$OUT_HTML" || exit 1

echo OK
