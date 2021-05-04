import { UPDATE_TABLES, UPDATE_PATHS } from "../actions/types";

const initialState = {
  tables: [],
  paths: {},
};

function tablesReducer(state = initialState, action) {
  const { type, payload } = action;

  switch (type) {
    case UPDATE_TABLES:
      return {
        ...state,
        tables: payload,
      };
    case UPDATE_PATHS:
      return {
        ...state,
        paths: payload,
      };
    default:
      return state;
  }
}

export default tablesReducer;
