import { SELECT_ROUTER, DESELECT_ROUTER } from "../actions/types";

export const selectRouter = () => async (dispatch) => {
  dispatch({ type: SELECT_ROUTER, Payload: undefined });
};

export const deselectRouter = () => async (dispatch) => {
  dispatch({ type: DESELECT_ROUTER, Payload: undefined });
};
