import React, { useState, useEffect } from "react";
import PropTypes from "prop-types";
import { connect } from "react-redux";

import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";

// Components
import CommandBar from "./components/CommandBar";
import RouterDisplay from "./components/RouterDisplay";
import InfoPanel from "./components/InfoPanel";

import "./App.css";

const App = ({ settings }) => {
  const [showPanel, setShowPanel] = useState(false);

  useEffect(() => {
    setShowPanel(settings.showInfoPanel);
  }, [settings.showInfoPanel]);

  return (
    <Container fluid>
      <Row style={{ height: "87vh" }}>
        <Col xs={showPanel ? 8 : 12} className='p-0'>
          <RouterDisplay />
        </Col>
        {showPanel && (
          <Col xs={4} className='bg-light p-1'>
            <InfoPanel />
          </Col>
        )}
      </Row>
      <Row style={{ height: "13vh" }}>
        <CommandBar />
      </Row>
    </Container>
  );
};

App.propTypes = {
  settings: PropTypes.object.isRequired,
};

const stateToProps = (state) => ({
  settings: state.settings,
});

export default connect(stateToProps, {})(App);
