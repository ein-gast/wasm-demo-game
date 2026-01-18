#!/usr/bin/env sh

. ./build.env || exit 1

# приписываем wasm к js:
node tools/stringify.js "$BUILD"/app.wasm > "$BUILD"/main.js  || exit 1
tail -n+2 "$SRC"/app_tpl.js >> "$BUILD"/main.js  || exit 1
# обновляем _dbg:
sed -e 's/main\.s\.js/main.js/' "$SRC"/html_tpl.html > "$BUILD"/app_dbg.html
echo DBG
