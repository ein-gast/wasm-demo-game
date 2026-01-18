const fs = require( "fs" );
const inline = require( "web-resource-inliner" );

const sourceFile = process.argv[process.argv.length-2] // ~ app.html
const contentPrefix = process.argv[process.argv.length-1] // ~ ./build

const data = fs.readFileSync(sourceFile);
const text = new TextDecoder('UTF-8').decode(data)

inline.html( {
    fileContent: text,
    relativeTo: contentPrefix
  },
  function( err, result ) { console.log(result) }
);


