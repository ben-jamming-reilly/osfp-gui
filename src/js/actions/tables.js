import { UPDATE_TABLES, UPDATE_PATHS } from "./types";
import store from "../store";

export const updateTables = () => async (dispatch) => {
  // Call on the OSFP FUNCTION HERE
  const state = store.getState();

  const tables = electron.osfpAPI.forwardingTable(state.topology.graph);

  //console.log(JSON.parse(tables));

  dispatch({
    type: UPDATE_TABLES,
    payload: JSON.parse(tables).forwardingTable,
  });
};

export const updatePaths = () => async (dispatch) => {
  const state = store.getState();
  const paths = JSON.parse(
    electron.osfpAPI.leastCostPathsTable(state.topology.graph)
  ).lowestCostPaths;

  console.log(paths);
  dispatch({
    type: UPDATE_PATHS,
    payload: paths,
  });
};
