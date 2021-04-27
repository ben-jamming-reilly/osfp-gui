import React from "react";
import PropTypes from "prop-types";
import { connect } from "react-redux";

import NavBar from "react-bootstrap/Navbar";
import NavbarBrand from "react-bootstrap/NavbarBrand";
import Nav from "react-bootstrap/Nav";
import Button from "react-bootstrap/Button";

import { addRouter } from "../actions/topology";

const CommandBar = ({ addRouter }) => {
  return (
    <NavBar bg='primary' variant='dark'>
      <NavbarBrand>
        <h3>OSPF</h3>
      </NavbarBrand>
      <Nav className='justify-content-end' style={{ width: "100%" }}>
        <Nav.Item>
          <Button variant='success' onClick={() => addRouter()}>
            <h5>Add Router</h5>
          </Button>
          <Button variant='danger' onClick={() => addRouter()}>
            <h5>Remove Router</h5>
          </Button>
        </Nav.Item>
      </Nav>
    </NavBar>
  );
};

CommandBar.propTypes = {
  addRouter: PropTypes.func.isRequired,
};

const stateToProps = (state) => ({});

export default connect(stateToProps, { addRouter })(CommandBar);
