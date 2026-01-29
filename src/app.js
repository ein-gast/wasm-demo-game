let _C, _T, S = 150, BS = S * S * 4, _E = inst.exports
_E.memory.grow(Math.ceil(1 + BS / 16 / 1024))
//console.log(inst.exports)
const pixPtr = _E.__heap_base.value; // __heap_base .. __heap_end
const pixBuf = new Uint8Array(mem = _E.memory.buffer).subarray(pixPtr, pixPtr + BS)
const image = (_T = (_C = document.getElementById("c"))
    .getContext("2d", { alpha: 0 }))
    .getImageData(0, 0, S, _C.width = _C.height = S);
_C.style = "width:min(90vw,90vh);height:min(90vw,90vh);image-rendering:pixelated;"
// ---
_E.init(S)
// обработка физики
setInterval((_) => { _E.process(0) }, 1000 / 60);
// ввод
_C.onmousedown = document.onkeydown = (_) => {
    _E.input(0, 0)
}
// рендер
const processFrame = (t) => {
    _E.render(t, pixPtr)
    // const ret = _E.render(t, pixPtr)
    // console.log(ret)
    image.data.set(pixBuf)
    _T.putImageData(image, 0, 0)
    requestAnimationFrame(processFrame);
}
processFrame(0)