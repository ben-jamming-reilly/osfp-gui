import React, { useState, useEffect } from "react";
import PropTypes from "prop-types";
import { connect } from "react-redux";

import ListGroup from "react-bootstrap/ListGroup";
import ListGroupItem from "react-bootstrap/ListGroupItem";
import InputGroup from "react-bootstrap/InputGroup";
import FormControl from "react-bootstrap/FormControl";
import Button from "react-bootstrap/Button";

import ArrowRepeat from "../icons//ArrowRepeat";

import { adjustWeights } from "../actions/topology";

const TopologyInfo = ({ topology, settings, adjustWeights }) => {
  const [newGraph, setNewGraph] = useState([]);
  const [selectedRouter, setSelectedRouter] = useState(0);

  useEffect(() => {
    setNewGraph(topology.graph);
    setSelectedRouter(settings.selectedRouter);
  }, [settings, topology.graph]);

  const onWeightChange = (e) => {
    setNewGraph(
      newGraph.map((tuple) =>
        (String(tuple[0]) === e.target.name && tuple[1] === selectedRouter) ||
        (String(tuple[1]) === e.target.name && tuple[0] === selectedRouter)
          ? [tuple[0], tuple[1], e.target.value]
          : tuple
      )
    );
  };

  const submitChanges = () => {
    adjustWeights(newGraph);
  };

  return (
    <div>
      <ListGroup
        variant='flush'
        style={{
          overflow: "auto",
          maxHeight: "55vh",
          marginBottom: "10px",
          WebkitOverflowScrolling: "touch",
        }}
      >
        {newGraph
          .filter((tuple) => tuple[0] == selectedRouter)
          .map((tuple) => (
            <ListGroupItem key={tuple[1]} className='py-auto px-0'>
              <InputGroup size='sm'>
                <InputGroup.Prepend>
                  <InputGroup.Text>Router {tuple[1]}</InputGroup.Text>
                </InputGroup.Prepend>
                <FormControl
                  type='number'
                  name={tuple[1]}
                  value={tuple[2]}
                  onChange={(e) => onWeightChange(e)}
                  min={0}
                />
              </InputGroup>
            </ListGroupItem>
          ))}
      </ListGroup>
      <Button style={{ width: "100%" }} onClick={() => submitChanges()}>
        <ArrowRepeat /> Update
      </Button>
    </div>
  );
};

TopologyInfo.propTypes = {
  topology: PropTypes.object.isRequired,
  settings: PropTypes.object.isRequired,
  adjustWeights: PropTypes.func.isRequired,
};

const stateToProps = (state) => ({
  topology: state.topology,
  settings: state.settings,
});

export default connect(stateToProps, { adjustWeights })(TopologyInfo);
