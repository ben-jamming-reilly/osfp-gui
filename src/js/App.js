import React from "react";

import { Provider } from "react-redux";
import store from "./store";

import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";

// Components
import CommandBar from "./components/CommandBar";

const App = () => {
  return (
    <Provider store={store}>
      <Container fluid>
        <Row>
          <Col xs='9'>a</Col>
          <Col xs='3'>b</Col>
        </Row>
        <Row>
          <CommandBar />
        </Row>
      </Container>
    </Provider>
  );
};

export default App;
