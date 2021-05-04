import { ADD_ROUTER, REMOVE_ROUTER, ADJUST_WEIGHTS } from "../actions/types";

const initialState = {
  graph: [
    [1, 2, 13],
    [2, 1, 13],
    //
    [1, 3, 13],
    [3, 1, 13],
    //
    [1, 4, 13],
    [4, 1, 13],
    //
    [1, 5, 13],
    [5, 1, 13],
    //--------
    [2, 3, 13],
    [3, 2, 13],
    //
    [2, 4, 13],
    [4, 2, 13],
    //
    [2, 5, 13],
    [5, 2, 13],
    //--------
    [3, 4, 13],
    [4, 3, 13],
    //
    [3, 5, 13],
    [5, 3, 13],
    //--------
    [4, 5, 13],
    [5, 4, 13],
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
