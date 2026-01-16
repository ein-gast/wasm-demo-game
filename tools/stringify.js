const fs = require('node:fs');

const data = fs.readFileSync(process.argv[process.argv.length - 1]);

console.log(
    `const code = new Uint8Array([${new Uint8Array(data).toString()}])`
)