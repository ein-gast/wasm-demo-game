const fs = require( "fs" );
const inline = require( "web-resource-inliner" );

const data = fs.readFileSync(process.argv[process.argv.length-1]);
const text = new TextDecoder('UTF-8').decode(data)

inline.html( {
    fileContent: text,
    relativeTo: "./"
  },
  function( err, result ) { console.log(result) }
);


