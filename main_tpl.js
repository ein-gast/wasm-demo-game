const code = new Uint8Array([0, 0, 0])
let _C, _T, S = 50, BS = S * S * 4
WebAssembly.instantiate(code).then(({ instance }) => {
    const pixPtr = Number(instance.exports.__heap_base);
    console.log(instance.exports, pixPtr);
    const pixBuf = new Uint8Array(instance.exports.memory.buffer).subarray(pixPtr, pixPtr + BS)
    const image = (_T = (_C = document.getElementById("c"))
        .getContext("2d", { alpha: 0 }))
        .getImageData(0, 0, S, _C.width = _C.height = S);

    document.getElementById("answ").innerText = instance.exports.add(0, S);
    const processFrame = (t) => {
        requestAnimationFrame(processFrame);
        instance.exports.fillBuff(t, 255, 0, 0, BigInt(pixPtr), S, S)
        image.data.set(pixBuf)
        _T.putImageData(image, 0, 0)
    }
    processFrame(0)

});