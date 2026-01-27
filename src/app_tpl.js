const Z = new Uint8Array([0, 0, 0]);
WebAssembly.instantiate(Z, {
    env: {
        extLog: function (num) {
            console.log(num)
        }
    }
})
    .then(({ instance:inst }) => {
        //console.log(instance.exports.app_js.value, instance.exports.memory.buffer)
        let str = '', mem = new Uint8Array(inst.exports.memory.buffer)
        for (let i = inst.exports.app_js.value; mem[i] !== 0; i++) {
            str += String.fromCharCode(mem[i])
        }
        eval(str)
    });
