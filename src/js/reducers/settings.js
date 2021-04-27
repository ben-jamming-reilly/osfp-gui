import { SELECT_ROUTER, DESELECT_ROUTER } from "../actions/types";

const initialState = {
  showInfoPanel: false,
};

function settingsReducer(state = initialState, action) {
  const { type, payload } = action;

  switch (type) {
    case SELECT_ROUTER:
      return { showInfoPanel: true };
    case DESELECT_ROUTER:
      return { showInfoPanel: false };
    default:
      return state;
  }
}

export default settingsReducer;
