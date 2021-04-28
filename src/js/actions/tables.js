import { UPDATE_TABLES } from "./types";
import store from "../store";

export const updateTables = () => async (dispatch) => {
  let graph = store.getState().topology.graph;

  // Call on the OSFP FUNCTION HERE

  // Remember that the graph will be updated
  dispatch({
    type: UPDATE_TABLES,
    payload: graph,
  });
};
