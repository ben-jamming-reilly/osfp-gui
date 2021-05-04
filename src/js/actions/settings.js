import {
  SELECT_ROUTER,
  DESELECT_ROUTER,
  SET_LINK_VIEW,
} from "../actions/types";

import store from "../store";

export const selectRouter = (router_id) => async (dispatch) => {
  dispatch({ type: SELECT_ROUTER, payload: Number(router_id) });
};

export const deselectRouter = () => async (dispatch) => {
  dispatch({ type: DESELECT_ROUTER, payload: undefined });
};

export const setLinkView = (view) => async (dispatch) => {
  if (view === "paths") {
    const state = store.getState();

    electron.osfpAPI.forwardingTable(
      state.topology.graph,
      state.settings.selectedRouter
    );
  }

  dispatch({ type: SET_LINK_VIEW, payload: view });
};
