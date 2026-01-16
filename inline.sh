#!/usr/bin/env sh

# приписываем wasm к js:
node tools/stringify.js add.wasm > add.html  || exit 1
tail -n+1 add_tpl.html >> add.html  || exit 1
# минимизируем js:
npx uglifyjs main.js > main.u.js  || exit 1
npx regpack main.u.js | tail -n+2 > main.s.js || exit 1
# встраиваем js в html:
node tools/inline.js add_tpl.html > add.html  || exit 1
echo OK
