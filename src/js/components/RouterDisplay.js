import React, { useState, useEffect, useRef, Fragment } from "react";
import PropTypes from "prop-types";
import { connect } from "react-redux";

import { Line } from "react-lineto";

import Router from "./Router";

const useWindowSize = () => {
  const [windowSize, setWindowSize] = useState({
    width: 0,
    height: 0,
  });

  useEffect(() => {
    function handleResize() {
      setWindowSize({
        width: window.innerWidth,
        height: window.innerHeight,
      });
    }
    window.addEventListener("resize", handleResize);

    handleResize();

    return () => window.removeEventListener("resize", handleResize);
  }, []);

  return windowSize;
};

const RouterDisplay = ({ topology }) => {
  let arr = [];

  const compRef = useRef(null);
  const winSize = useWindowSize();
  const [numRouters, setNumRouters] = useState(topology.size);

  const [dim, setDim] = useState({
    height: 0,
    width: 0,
  });

  for (let i = 0; i < numRouters; i++) arr.push(i + 1);

  useEffect(() => {
    function handleResize() {
      setDim({
        height: compRef.current.clientHeight,
        width: compRef.current.clientWidth,
      });
    }
    window.addEventListener("resize", handleResize);
    setDim({
      height: compRef.current.clientHeight,
      width: compRef.current.clientWidth,
    });

    return () => window.removeEventListener("resize", handleResize);
  }, [winSize]);

  useEffect(() => {
    arr = [];
    for (let i = 0; i < numRouters; i++) arr.push(i + 1);
  }, [topology.size]);

  const xCoord = (grad, scalar) =>
    Math.round(
      scalar * (dim.width / 2) * Math.cos(2 * Math.PI * grad - Math.PI / 2) +
        dim.width / 2
    );

  const yCoord = (grad, scalar) =>
    Math.round(
      scalar * (dim.height / 2) * Math.sin(2 * Math.PI * grad - Math.PI / 2) +
        dim.height / 2
    );

  return (
    <div ref={compRef} style={{ width: "100%", height: "100%" }}>
      <div>
        {arr.map((elem) => (
          <Router
            className={`router-${elem}`}
            key={elem}
            name={`${elem}`}
            x={xCoord((elem - 1) / arr.length, 0.7)}
            y={yCoord((elem - 1) / arr.length, 0.7)}
          />
        ))}
      </div>
      <div>
        {arr.map((elem) => (
          <Line
            zIndex={0}
            key={elem}
            borderColor='#0275d8'
            borderWidth={3}
            x0={xCoord((elem - 1) / arr.length, 0.7)}
            y0={yCoord((elem - 1) / arr.length, 0.7)}
            x1={xCoord(elem / arr.length, 0.7)}
            y1={yCoord(elem / arr.length, 0.7)}
          />
        ))}
      </div>
    </div>
  );
};

RouterDisplay.propTypes = {
  topology: PropTypes.object.isRequired,
};

const stateToProps = (state) => ({
  topology: state.topology,
});

export default connect(stateToProps, {})(RouterDisplay);
