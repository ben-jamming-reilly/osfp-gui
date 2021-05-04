const { ipcRenderer, contextBridge } = require("electron");

const native_ospf = require("./build/Release/native_ospf.node");

contextBridge.exposeInMainWorld("electron", {
  notificationApi: {
    sendNotification(msg) {
      console.log(msg);
      ipcRenderer.send("notify", msg);
    },
  },
  osfpAPI: {
    forwardingTable(data, router) {
      const graph_str = JSON.stringify({ networkTopology: data });

      console.log(graph_str);

      return native_ospf.getForwardingTable(String(graph_str));
    },
    leastCostPathsTable(data) {
      console.log(data);
      //getLeastCostPathsTable()
    },
  },
});
