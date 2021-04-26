import React, { useState } from "react";

import { Provider } from "react-redux";
import store from "./store";

import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";

// Components
import CommandBar from "./components/CommandBar";
import RouterDisplay from "./components/RouterDisplay";
import InfoPanel from "./components/InfoPanel";

import "./App.css";

const App = () => {
  return (
    <Provider store={store}>
      <Container fluid>
        <Row style={{ height: "87vh" }}>
          <Col xs={8} className='p-0'>
            <RouterDisplay />
          </Col>
          <Col xs={4} className='bg-light p-1'>
            <InfoPanel />
          </Col>
        </Row>
        <Row style={{ height: "13vh" }}>
          <CommandBar />
        </Row>
      </Container>
    </Provider>
  );
};

export default App;
