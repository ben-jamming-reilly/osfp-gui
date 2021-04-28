import React from "react";

import Table from "react-bootstrap/Table";

const TablesInfo = () => {
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
          <th>Router</th>
          <th>Destination</th>
          <th>First Hop</th>
          <th>Cost</th>
        </thead>
        <tbody>
          <tr>
            <td>1</td>
            <td>X</td>
            <td>Y</td>
            <td>420</td>
          </tr>
        </tbody>
      </Table>
    </div>
  );
};

export default TablesInfo;
