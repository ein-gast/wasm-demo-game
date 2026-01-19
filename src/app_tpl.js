new Blob([new Uint8Array([0, 0, 0])]).stream().pipeThrough(new DecompressionStream("gzip")).getReader().read().then((code) => {// эта строка заменяется реальным кодом при сборке
    let _C, _T, S = 150, BS = S * S * 4
    WebAssembly.instantiate(code.value)
        .then(({ instance }) => {
            instance.exports.memory.grow(Math.ceil(1 + BS / 16 / 1024))
            console.log(instance.exports)
            const pixPtr = instance.exports.__heap_base.value; // __heap_base .. __heap_end
            const pixBuf = new Uint8Array(instance.exports.memory.buffer).subarray(pixPtr, pixPtr + BS)
            const image = (_T = (_C = document.getElementById("c"))
                .getContext("2d", { alpha: 0 }))
                .getImageData(0, 0, S, _C.width = _C.height = S);
            // ---
            instance.exports.init(S, S)
            // обработка физики
            setInterval(() => { instance.exports.process(0) }, 1000 / 60);
            // ввод
            _C.onmousedown = document.onkeydown = () => {
                instance.exports.input(0, 0)
            }
            // рендер
            const processFrame = (t) => {
                instance.exports.render(t, pixPtr)
                // const ret = instance.exports.render(t, pixPtr)
                // console.log(ret)
                image.data.set(pixBuf)
                _T.putImageData(image, 0, 0)
                requestAnimationFrame(processFrame);
            }
            processFrame(0)
        });
});