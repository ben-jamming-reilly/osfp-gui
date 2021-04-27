import { ADD_ROUTER, ADJUST_WEIGHTS, REMOVE_ROUTER } from "./types";

export const addRouter = () => async (dispatch) => {
  dispatch({ type: ADD_ROUTER, payload: undefined });
};

export const removeRouter = () => async (dispatch) => {
  dispatch({ type: REMOVE_ROUTER, payload: undefined });
};

export const adjustWeights = () => async (dispatch) => {};
