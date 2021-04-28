import { ADD_ROUTER, REMOVE_ROUTER, ADJUST_WEIGHTS } from "../actions/types";

const initialState = {
  graph: [
    [1, 2, 99],
    [2, 1, 99],
    //
    [1, 3, 99],
    [3, 1, 99],
    //
    [1, 4, 99],
    [4, 1, 99],
    //
    [1, 5, 99],
    [5, 1, 99],
    //--------
    [2, 3, 99],
    [3, 2, 99],
    //
    [2, 4, 99],
    [4, 2, 99],
    //
    [2, 5, 99],
    [5, 2, 99],
    //--------
    [3, 4, 99],
    [4, 3, 99],
    //
    [3, 5, 99],
    [5, 3, 99],
    //--------
    [4, 5, 99],
    [5, 4, 99],
  ],
  size: 5,
};

function topologyReducer(state = initialState, action) {
  const { type, payload } = action;

  switch (type) {
    case ADD_ROUTER:
      return {
        graph: payload,
        size: state.size + 1,
      };
    case REMOVE_ROUTER:
      return {
        graph: payload,
        size: state.size - 1,
      };
    case ADJUST_WEIGHTS:
      return {
        ...state,
        graph: payload,
      };
    default:
      return state;
  }
}

export default topologyReducer;
