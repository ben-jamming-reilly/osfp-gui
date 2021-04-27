import React, { Fragment, useState, useEffect, useRef } from "react";
import PropTypes from "prop-types";
import { connect } from "react-redux";

import XCircle from "../icons/XCircle";
import Button from "react-bootstrap/Button";
import Row from "react-bootstrap/Row";

import { selectRouter } from "../actions/settings";

const Router = ({ selectRouter, x, y, name }) => {
  const compRef = useRef(null);

  useEffect(() => {
    compRef.current.style.top = `${y - compRef.current.clientHeight / 2}px`;
    compRef.current.style.left = `${x - compRef.current.clientWidth / 2}px`;
  }, [x, y]);

  return (
    <Fragment>
      {x > 10 && y > 10 && (
        <div
          className='position-absolute'
          ref={compRef}
          style={{ height: "50px", width: "50px", zIndex: "100" }}
        >
          <Button
            className=''
            variant='primary'
            style={{ borderRadius: "50px" }}
            onClick={() => selectRouter()}
          >
            <Row className='justify-content-center'>
              {name}
              <XCircle />
            </Row>
          </Button>
        </div>
      )}
    </Fragment>
  );
};

Router.propTypes = {
  selectRouter: PropTypes.func.isRequired,
};

const stateToProps = (state) => ({});

export default connect(stateToProps, { selectRouter })(Router);
