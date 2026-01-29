// Math.ceil(1 + BS / 16 / 1024) = 7
//console.log(inst.exports)
let _C, _T, S = 150, BS = S * S * 4, _E = inst.exports,
    image = (_T = (_C = document.getElementById(`c`, _E.memory.grow(7 /*Math.ceil(1 + BS / 16 / 1024)*/)))
        .getContext(`2d`))
        .getImageData(0, 0, S, _C.width = _C.height = S),
    pixPtr = _E.__heap_base.value, // __heap_base .. __heap_end
    pixBuf = new Uint8Array(mem = _E.memory.buffer).subarray(pixPtr, pixPtr + BS),
    WH90 = `min(90vw,90vh)`;
_C.style = `width:${WH90};height:${WH90};image-rendering:pixelated`
// ---

_E.init(S)
// обработка физики
setInterval((_) => _E.process(), 16 /*1000/60=16*/);
// ввод
_C.onmousedown = document.onkeydown = (_) => _E.input()
// рендер
let processFrame = (_) => {
    _E.putImageData(pixPtr)
    // const ret = _E.render(t, pixPtr)
    // console.log(ret)
    image.data.set(pixBuf)
    _T.putImageData(image, 0, 0)
    requestAnimationFrame(processFrame);
}
processFrame(0)