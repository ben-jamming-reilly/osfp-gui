import { ADD_ROUTER, REMOVE_ROUTER, ADJUST_WEIGHTS } from "../actions/types";

const initialState = {
  graph: [
    {
      start: 0,
      end: 1,
      weight: 1,
    },
    {
      start: 0,
      end: 2,
      weight: 2,
    },
    {
      start: 0,
      end: 3,
      weight: 3,
    },
    {
      start: 0,
      end: 4,
      weight: 4,
    },
    {
      start: 1,
      end: 2,
      weight: 5,
    },
    {
      start: 1,
      end: 3,
      weight: 6,
    },
    {
      start: 1,
      end: 4,
      weight: 7,
    },
    {
      start: 2,
      end: 3,
      weight: 8,
    },
    {
      start: 2,
      end: 4,
      weight: 9,
    },
    {
      start: 3,
      end: 4,
      weight: 10,
    },
  ],
  size: 5,
};

function topologyReducer(state = initialState, action) {
  const { type, payload } = action;

  switch (type) {
    case ADD_ROUTER:
      return {
        ...state, //graph: payload.graph,
        size: state.size + 1,
      };
    case REMOVE_ROUTER:
      return {
        ...state, //graph: payload.graph,
        size: state.size--,
      };
    case ADJUST_WEIGHTS:
      return {
        ...state,
        graph: payload.graph,
      };
    default:
      return state;
  }
}

export default topologyReducer;
