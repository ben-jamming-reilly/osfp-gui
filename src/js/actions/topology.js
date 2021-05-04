import { ADD_ROUTER, ADJUST_WEIGHTS, REMOVE_ROUTER } from "./types";
import store from "../store";
import { updateTables, updatePaths } from "./tables";

export const addRouter = () => async (dispatch) => {
  let topology = store.getState().topology;
  const new_router_id = topology.size + 1;

  // I'm hard coding in 99 for the default weight for rn
  for (let i = 1; i <= topology.size; i++) {
    let tuple1 = [i, new_router_id, 13];
    let tuple2 = [new_router_id, i, 13];

    topology.graph.push(tuple1);
    topology.graph.push(tuple2);
  }
  dispatch({ type: ADD_ROUTER, payload: topology.graph });

  dispatch(updateTables());
  dispatch(updatePaths());
};

export const removeRouter = () => async (dispatch) => {
  let topology = store.getState().topology;
  const removed_router_id = topology.size;

  dispatch({
    type: REMOVE_ROUTER,
    payload: topology.graph.filter(
      (tuple) =>
        tuple[0] !== removed_router_id && tuple[1] !== removed_router_id
    ),
  });

  dispatch(updateTables());
  dispatch(updatePaths());
};

export const adjustWeights = (newGraph) => async (dispatch) => {
  dispatch({ type: ADJUST_WEIGHTS, payload: newGraph });

  //dispatch(updateTables());
  //dispatch(updatePaths());
};
