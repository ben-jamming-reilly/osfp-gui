import {
  SELECT_ROUTER,
  DESELECT_ROUTER,
  SET_LINK_VIEW,
} from "../actions/types";

const initialState = {
  showInfoPanel: false,
  selectedRouter: 0,
  linkView: "",
};

function settingsReducer(state = initialState, action) {
  const { type, payload } = action;

  switch (type) {
    case SELECT_ROUTER:
      return { showInfoPanel: true, selectedRouter: payload };
    case DESELECT_ROUTER:
      return { showInfoPanel: false, selectedRouter: 0 };
    case SET_LINK_VIEW:
      return {
        ...state,
        linkView: payload,
      };
    default:
      return state;
  }
}

export default settingsReducer;
