import { SELECT_ROUTER, DESELECT_ROUTER } from "../actions/types";

export const selectRouter = (router_id) => async (dispatch) => {
  dispatch({ type: SELECT_ROUTER, payload: Number(router_id) });
};

export const deselectRouter = () => async (dispatch) => {
  dispatch({ type: DESELECT_ROUTER, payload: undefined });
};
