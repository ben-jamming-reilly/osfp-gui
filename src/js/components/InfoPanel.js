import React, { useState } from "react";

import Card from "react-bootstrap/Card";
import ButtonGroup from "react-bootstrap/ButtonGroup";
import Button from "react-bootstrap/Button";

import GraphIcon from "../icons/Graph";
import TableIcon from "../icons/Table";

const InfoPanel = () => {
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
      <Card.Body>{isTableView ? "table" : "Topology"}</Card.Body>
    </Card>
  );
};

export default InfoPanel;
