#!/usr/bin/env sh

. ./build.env || exit 1

# приписываем wasm к js:
#node tools/stringify.js "$BUILD"/app.wasm > "$BUILD"/main.js  || exit 1
node tools/stringify.js "$BUILD"/app.wasm.gz > "$BUILD"/main.js  || exit 1
tail -n+2 "$SRC"/app_tpl.js >> "$BUILD"/main.js  || exit 1
# обновляем _dbg:
sed -e 's/main\.s\.js/main.js/' "$SRC"/html_tpl.html > "$BUILD"/app_tpl.html
node tools/inline.js "$BUILD"/app_tpl.html "$BUILD" > "$BUILD"/app_dbg.html  || exit 1
rm "$BUILD"/app_tpl.html
#sed -i -e 's/<script/<script type=module/' "$BUILD"/app_dbg.html
echo DBG
