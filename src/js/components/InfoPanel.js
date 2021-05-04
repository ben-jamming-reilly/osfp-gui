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
import ArrowBoxRight from "../icons/ArrowBoxRight";

import { deselectRouter, setLinkView } from "../actions/settings";

const InfoPanel = ({ deselectRouter, setLinkView, settings }) => {
  const [isTableView, setIsTableView] = useState(false);

  return (
    <Card style={{ width: "100%", height: "100%" }}>
      <Card.Header className='text-center'>
        <ButtonGroup>
          <Button
            variant={!isTableView ? "primary" : "outline-primary"}
            onClick={() => {
              setIsTableView(false);
              setLinkView("connections");
            }}
          >
            <GraphIcon /> Topology
          </Button>
          <Button
            variant={isTableView ? "primary" : "outline-primary"}
            onClick={() => {
              setIsTableView(true);
              setLinkView("paths");
            }}
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
          <ArrowBoxRight />
        </Button>
      </Card.Footer>
    </Card>
  );
};

InfoPanel.propTypes = {
  deselectRouter: PropTypes.func.isRequired,
  setLinkView: PropTypes.func.isRequired,
  settings: PropTypes.object.isRequired,
};

const stateToProps = (state) => ({
  settings: state.settings,
});

export default connect(stateToProps, { deselectRouter, setLinkView })(
  InfoPanel
);
