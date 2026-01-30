const fs = require('node:fs');

const fileName = process.argv[process.argv.length - 1];
const data = fs.readFileSync(fileName);

if (fileName.endsWith(".gz")) {
    console.log(
        `U=Uint8Array;new Blob([new U([${new Uint8Array(data).toString()}])])` +
        `.stream().pipeThrough(new DecompressionStream("gzip"))` +
        `.getReader().read().then((z)=>{Z=z.value;`,
        "\n})",
    )
    return 0;
}

if (fileName.endsWith(".base64")) {
    console.log(
        `U = Uint8Array; Z = U.from(atob('${data.toString()}'), c => c.charCodeAt(0))`,
        "\n/*-*/",
    )
    return 0;
}

if (fileName.endsWith(".wasm")) {

    console.log(
        `U = Uint8Array; Z = new U([${new Uint8Array(data).toString()}]);`,
        // 'const code = new Uint8Array(' + JSON.stringify(encodeRLE1(new Uint8Array(data))) + '.reduce((res,val)=>res.concat(val[0] ? Array(val[0]).fill(val[1]) : val.slice(1)),[]));',
        // 'const code = new Uint8Array(' + JSON.stringify(encodeRLE2(new Uint8Array(data))) + '.reduce((res,val)=>res.concat(typeof(val)==="object"?Array(val[0]).fill(val[1]):[val]),[]));',
        "\n/*-*/",
    )
    return 0;
}

console.error("Не указан входной файл")
return -1;

//console.log(JSON.stringify(encodeRLE([1, 2, 3, 3, 3, 3, 4, 5])))
//-------------------------------
// RLE1 format: 
// [[0,2,3,4],[2,0],[3,1],[0,1,2,3]] -> [2,3,4  0,0, 1,1,1, 1,2,3]
function encodeRLE1(arr) {
    const reducerA = (acc, val) => {
        //console.log(acc)
        if (acc.length === 0) {
            acc.push([0, val])
            return acc;
        }
        const prev = acc[acc.length - 1];
        if (prev[prev.length - 1] === val && prev.length === 2) {
            prev[0] += prev[0] ? 1 : 2;
            acc[acc.length - 1] = prev
            return acc;
        }
        if (prev[prev.length - 1] === val && prev.length > 2) {
            acc[acc.length - 1] = prev.slice(0, -1)
            acc.push([2, val]);
            return acc;
        }
        if (prev.length === 2 && prev[0] > 0) {
            acc.push([0, val])
            return acc;
        }
        prev.push(val);
        acc[acc.length - 1] = prev
        return acc;
    };
    return arr.reduce(reducerA, []);
}

function decoder1(res, val) {
    return res.concat(val[0] ? Array(val[0]).fill(val[1]) : val.slice(1))
}

// RLE2 format: 
// [0,2,3,4,[2,0],[3,1],0,1,2,3] -> [2,3,4  0,0, 1,1,1, 1,2,3]
function encodeRLE2(arr) {
    const reducerA = (acc, val) => {
        if (acc.length === 0) {
            acc.push([0, val])
            return acc;
        }
        const prev = acc[acc.length - 1];
        if (prev[prev.length - 1] === val && prev.length === 2) {
            prev[0] += prev[0] ? 1 : 2;
            acc[acc.length - 1] = prev
            return acc;
        }
        if (prev[prev.length - 1] === val && prev.length > 2) {
            acc[acc.length - 1] = prev.slice(0, -1)
            acc.push([2, val]);
            return acc;
        }
        if (prev.length === 2 && prev[0] > 0) {
            acc.push([0, val])
            return acc;
        }
        prev.push(val);
        acc[acc.length - 1] = prev
        return acc;
    };
    const reducerB = (acc, val) => {
        if (val[0] > 0) {
            if (val[0] === 1) {
                acc.push(val[1])
            } else {
                acc.push(val)
            }
            return acc
        } else {
            return acc.concat(val.slice(1))
        }
    }
    return arr.reduce(reducerA, []).reduce(reducerB, []);
}

function decoder2(res, val) {
    return res.concat(typeof (val) === "object" ? Array(val[0]).fill(val[1]) : [val])
}

