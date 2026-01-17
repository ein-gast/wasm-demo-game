const code = new Uint8Array([0, 0, 0])
let _C, _T, S = 150, BS = S * S * 4
WebAssembly.instantiate(code)
    .then(({ instance }) => {
        instance.exports.memory.grow(BigInt(Math.ceil(1 + BS / 16 / 1024)))
        const pixPtr = Number(instance.exports.__heap_base); // __heap_base - __heap_end
        const pixBuf = new Uint8Array(instance.exports.memory.buffer).subarray(pixPtr, pixPtr + BS)
        const image = (_T = (_C = document.getElementById("c"))
            .getContext("2d", { alpha: 0 }))
            .getImageData(0, 0, S, _C.width = _C.height = S);
        //console.log(instance.exports, _C);
        instance.exports.init()
        _C.onmousedown = document.onkeydown = () => { instance.exports.input(0, 0) }
        const processFrame = (t) => {
            instance.exports.render(t, 255, 0, 0, BigInt(pixPtr), S, S)
            image.data.set(pixBuf)
            _T.putImageData(image, 0, 0)
            requestAnimationFrame(processFrame);
        }
        processFrame(0)

    });