const native_ospf = require('./build/Release/native_ospf.node')

const stringified_json = "{\"networkTopology\":[[0,1,5],[1,0,5],[0,2,11],[2,0,11],[1,2,1],[2,1,1]]}";

console.log(native_ospf)
console.log()
console.log(stringified_json)
console.log()
console.log(native_ospf.getForwardingTable(stringified_json))
console.log()
