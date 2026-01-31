U = Uint8Array; Z = new U([0, 0, 0]);
WebAssembly.instantiate(Z)
    .then(({ instance: inst }) => {
        //console.log(instance.exports.js.value, instance.exports.memory.buffer)
        let i = inst.exports.js.value, S = '', m = new U(inst.exports.memory.buffer)
        for (; m[i] !== 0; i++) {
            S += String.fromCharCode(m[i])
        }
        eval(S)
    });
