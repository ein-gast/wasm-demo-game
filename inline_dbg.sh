#!/usr/bin/env sh

# приписываем wasm к js:
node tools/stringify.js add.wasm > main.js  || exit 1
tail -n+2 main_tpl.js >> main.js  || exit 1
# обновляем _dbg:
sed -e 's/main\.s\.js/main.js/' add_tpl.html > add_dbg.html
echo DBG
