const bytes = new Uint8Array([0,0,0])
WebAssembly.instantiate(bytes).then(( { instance })=>{
    console.log(instance.exports)
    document.getElementById("answ").innerText = instance.exports.add(1, 2);
});