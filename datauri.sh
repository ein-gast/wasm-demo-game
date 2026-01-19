#!/usr/bin/env sh

. ./build.env || exit 1

printf 'data:text/html;base64,' >  "$BUILD"/app.url.txt || exit 1
base64 -w 0 < "$BUILD"/app.html >> "$BUILD"/app.url.txt || exit 1
cat "$BUILD"/app.url.txt
echo
echo OK
