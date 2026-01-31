#!/usr/bin/env sh

. ./build.env || exit 1

OP="GZipBase64"
if test ! -z "$1"; then
    OP="$1"
fi

echo "$OP"

./compile.sh APP_MIN

case "$OP" in
"Uint8")
    OUT_HTML="$BUILD"/app_A.html
    OUT_MD="$BUILD"/app_A.url.md
    OUT_MD2="$BUILD"/app_A.url2.md
    node tools/stringify.js "$BUILD"/app.wasm >"$BUILD"/main0.js || exit 1
    ;;
"GZipArray")
    OUT_HTML="$BUILD"/app_G.html
    OUT_MD="$BUILD"/app_G.url.md
    OUT_MD2="$BUILD"/app_G.url2.md
    node tools/stringify.js "$BUILD"/app.wasm.gz >"$BUILD"/main0.js || exit 1
    ;;
"Base64")
    OUT_HTML="$BUILD"/app_B.html
    OUT_MD="$BUILD"/app_B.url.md
    OUT_MD2="$BUILD"/app_B.url2.md
    node tools/stringify.js "$BUILD"/app.wasm.base64 >"$BUILD"/main0.js || exit 1
    ;;
"GZipBase64")
    OUT_HTML="$BUILD"/app_Z.html
    OUT_MD="$BUILD"/app_Z.url.md
    OUT_MD2="$BUILD"/app_Z.url2.md
    node tools/stringify.js "$BUILD"/app.wasm.gz.base64 >"$BUILD"/main0.js || exit 1
    ;;
*)
    echo "??? $OP"
    exit 1
    ;;
esac

: >"$BUILD"/main-$OP.js

# приписываем wasm к js:
head -n 1 "$BUILD"/main0.js >"$BUILD"/main-$OP.js || exit 1
tail -n+2 "$SRC"/boot_tpl.js >>"$BUILD"/main-$OP.js || exit 1
tail -n 1 "$BUILD"/main0.js >>"$BUILD"/main-$OP.js || exit 1
rm "$BUILD"/main0.js

# минимизируем js:
npx uglifyjs --toplevel --keep-fargs --rename "$BUILD"/main-$OP.js >"$BUILD"/main.u-$OP.js || exit 1
npx regpack --reassignVars 0 "$BUILD"/main.u-$OP.js | sed -e 's/^stats:.*$//g' >"$BUILD"/main.z-$OP.js || exit 1

cp "$BUILD"/main.z-$OP.js "$BUILD"/main.z.js

# встраиваем js в html:
echo "$OUT_HTML"
node tools/inline.js "$SRC"/html_tpl.html "$BUILD" | tr -d '\r' | tr -d '\n' >"$OUT_HTML" || exit 1

# генерируем telegram markdown
printf '```text\ndata:text/html;base64,' >"$OUT_MD" || exit 1
base64 -w 0 <"$OUT_HTML" >>"$OUT_MD" || exit 1
echo >> "$OUT_MD" || exit 1
echo '```' >> "$OUT_MD" || exit 1

# генерируем telegram markdown2
printf '```text\ndata:text/html,<canvas%%20id=c></canvas><script>' > "$OUT_MD2" || exit 1
cat "$BUILD"/main.u-$OP.js >> "$OUT_MD2" || exit 1
echo '</script>' >> "$OUT_MD2" || exit 1
echo '```' >>"$OUT_MD2" || exit 1

echo OK
