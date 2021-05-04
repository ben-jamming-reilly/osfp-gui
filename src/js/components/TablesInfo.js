import React, { useState, useEffect } from "react";
import PropTypes from "prop-types";
import { connect } from "react-redux";

import Table from "react-bootstrap/Table";

const TablesInfo = ({ settings, tables }) => {
  const [tablesArr, setTablesArr] = useState([]);
  const [curRouter, setCurRouter] = useState(0);

  useEffect(() => {
    const index = settings.selectedRouter;

    console.log(index);
    setTablesArr(tables.tables[index]);
    setCurRouter(index);
  }, [settings, tables]);

  //console.log(tablesArr);

  return (
    <div className='text-center'>
      <Table
        striped
        size='sm'
        style={{
          overflow: "auto",
          maxHeight: "55vh",
          marginBottom: "10px",
          WebkitOverflowScrolling: "touch",
        }}
      >
        <thead>
          <tr>
            <th>Router</th>
            <th>Destination</th>
            <th>First Hop</th>
            <th>Cost</th>
          </tr>
        </thead>
        <tbody>
          {tablesArr.map((item) => (
            <tr key={Math.random() * 1000000}>
              <td>{curRouter}</td>
              <td>{item[0]}</td>
              <td>{item[1]}</td>
              <td>{item[2]}</td>
            </tr>
          ))}
        </tbody>
      </Table>
    </div>
  );
};

TablesInfo.propTypes = {
  tables: PropTypes.object.isRequired,
  settings: PropTypes.object.isRequired,
};

const stateToProps = (state) => ({
  tables: state.tables,
  settings: state.settings,
});

export default connect(stateToProps, {})(TablesInfo);
