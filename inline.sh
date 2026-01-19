#!/usr/bin/env sh

. ./build.env || exit 1

# приписываем wasm к js:
#node tools/stringify.js "$BUILD"/app.wasm > "$BUILD"/main.js  || exit 1
node tools/stringify.js "$BUILD"/app.wasm.gz > "$BUILD"/main.js  || exit 1
tail -n+2 "$SRC"/app_tpl.js >> "$BUILD"/main.js  || exit 1
# минимизируем js:
npx uglifyjs --rename "$BUILD"/main.js > "$BUILD"/main.u.js  || exit 1
npx regpack --es6 0 --reassignVars 0 "$BUILD"/main.u.js | tail -n+2 > "$BUILD"/main.s.js || exit 1
# встраиваем js в html:
node tools/inline.js "$SRC"/html_tpl.html "$BUILD" > "$BUILD"/app.html  || exit 1
#sed -i -e 's/<script/<script type=module/' "$BUILD"/app.html
echo OK
