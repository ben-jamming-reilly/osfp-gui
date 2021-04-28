import React, { useState } from "react";
import PropTypes from "prop-types";
import { connect } from "react-redux";

// sub-component
import TablesInfo from "./TablesInfo";
import TopologyInfo from "./TopologyInfo";

// bootstrap
import Card from "react-bootstrap/Card";
import ButtonGroup from "react-bootstrap/ButtonGroup";
import Button from "react-bootstrap/Button";

// icon
import GraphIcon from "../icons/Graph";
import TableIcon from "../icons/Table";
import ArrowBowRight from "../icons/ArrowBoxRight";

import { deselectRouter } from "../actions/settings";

const InfoPanel = ({ deselectRouter }) => {
  const [isTableView, setIsTableView] = useState(false);

  return (
    <Card style={{ width: "100%", height: "100%" }}>
      <Card.Header className='text-center'>
        <ButtonGroup>
          <Button
            variant={!isTableView ? "primary" : "outline-primary"}
            onClick={() => setIsTableView(false)}
          >
            <GraphIcon /> Topology
          </Button>
          <Button
            variant={isTableView ? "primary" : "outline-primary"}
            onClick={() => setIsTableView(true)}
          >
            <TableIcon /> Tables
          </Button>
        </ButtonGroup>
      </Card.Header>
      <Card.Body>{isTableView ? <TablesInfo /> : <TopologyInfo />}</Card.Body>
      <Card.Footer className='text-muted text-center my-0 py-1'>
        <Button
          variant='outline-danger'
          onClick={() => {
            deselectRouter();
          }}
        >
          <ArrowBowRight />
        </Button>
      </Card.Footer>
    </Card>
  );
};

InfoPanel.propTypes = {
  deselectRouter: PropTypes.func.isRequired,
};

const stateToProps = (state) => ({});

export default connect(stateToProps, { deselectRouter })(InfoPanel);
