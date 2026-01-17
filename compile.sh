#clang --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry -o add.wasm add.c || exit 1
clang --target=wasm64 --no-standard-libraries -Wl,--export-all -Wl,--no-entry -o add.wasm add.c || exit 1
chmod -x add.wasm

echo OK
