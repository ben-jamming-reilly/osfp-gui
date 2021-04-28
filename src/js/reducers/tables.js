const initialState = {};

function tablesReducer(state = initialState, action) {
  const { type, payload } = action;

  switch (type) {
    case "TEST":
      return state;
    default:
      return state;
  }
}

export default tablesReducer;
