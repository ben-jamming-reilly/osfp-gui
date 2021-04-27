import React from "react";
import PropTypes from "prop-types";

import NavBar from "react-bootstrap/Navbar";
import NavbarBrand from "react-bootstrap/NavbarBrand";
import Nav from "react-bootstrap/Nav";
import Button from "react-bootstrap/Button";

const CommandBar = () => {
  return (
    <NavBar bg='primary' variant='dark'>
      <NavbarBrand>
        <h3>OSPF</h3>
      </NavbarBrand>
      <Nav className='justify-content-end' style={{ width: "100%" }}>
        <Nav.Item>
          <Button variant='success'>
            <h5>Add Router</h5>
          </Button>
        </Nav.Item>
      </Nav>
    </NavBar>
  );
};

CommandBar.propTypes = {};

export default CommandBar;
