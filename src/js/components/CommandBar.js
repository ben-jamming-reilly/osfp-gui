import React from "react";
import PropTypes from "prop-types";

import NavBar from "react-bootstrap/Navbar";
import NavbarBrand from "react-bootstrap/NavbarBrand";

const CommandBar = () => {
  return (
    <NavBar bg='primary' variant='dark'>
      <NavbarBrand>
        <h3>OSPF</h3>
      </NavbarBrand>
    </NavBar>
  );
};

CommandBar.propTypes = {};

export default CommandBar;
