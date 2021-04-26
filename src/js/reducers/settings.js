const initialState = {
  displayRouterForm: false,
};

function settingsReducer(state = initialState, action) {
  const { type, payload } = action;

  switch (type) {
    case "TEST":
      return { displayRouterForm: true };
    default:
      return state;
  }
}
