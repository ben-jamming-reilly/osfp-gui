import { ADD_ROUTER, ADJUST_WEIGHTS, REMOVE_ROUTER } from "./types";

export const addRouter = () => async (dispatch) => {
  console.log("Router Added");
  dispatch({ type: ADD_ROUTER, payload: undefined });
};

export const removeRouter = () => async (dispatch) => {};

export const adjustWeights = () => async (dispatch) => {};
