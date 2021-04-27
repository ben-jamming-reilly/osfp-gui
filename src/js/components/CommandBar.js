import React from "react";
import PropTypes from "prop-types";
import { connect } from "react-redux";

import NavBar from "react-bootstrap/Navbar";
import NavbarBrand from "react-bootstrap/NavbarBrand";
import Nav from "react-bootstrap/Nav";
import Button from "react-bootstrap/Button";

import { addRouter, removeRouter } from "../actions/topology";

const CommandBar = ({ addRouter, removeRouter, topology }) => {
  return (
    <NavBar bg='primary' variant='dark'>
      <NavbarBrand>
        <h3>OSPF</h3>
      </NavbarBrand>
      <Nav className='justify-content-end' style={{ width: "100%" }}>
        <Nav.Item>
          <Button
            className='mx-1'
            variant='success'
            onClick={() => addRouter()}
          >
            Add Router
          </Button>
          {topology.size > 2 && (
            <Button
              className='mx-1'
              variant='danger'
              onClick={() => removeRouter()}
            >
              Remove Router
            </Button>
          )}
        </Nav.Item>
      </Nav>
    </NavBar>
  );
};

CommandBar.propTypes = {
  addRouter: PropTypes.func.isRequired,
  removeRouter: PropTypes.func.isRequired,
  topology: PropTypes.object.isRequired,
};

const stateToProps = (state) => ({
  topology: state.topology,
});

export default connect(stateToProps, { addRouter, removeRouter })(CommandBar);
