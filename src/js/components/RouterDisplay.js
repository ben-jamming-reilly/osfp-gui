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

const RouterDisplay = ({ topology, settings }) => {
  const compRef = useRef(null);
  const winSize = useWindowSize();

  const [numRouters, setNumRouters] = useState(topology.size);
  const [selectedRouter, setSelectedRouter] = useState(0);
  const [linkView, setLinkView] = useState("");

  const [arr, setArr] = useState([]);
  const [dim, setDim] = useState({
    height: 0,
    width: 0,
  });

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
  }, [winSize, settings]);

  useEffect(() => {
    setSelectedRouter(settings.selectedRouter);
    setLinkView(settings.linkView);
  }, [settings]);

  useEffect(() => {
    let newArr = [];
    for (let i = 1; i <= topology.size; i++) newArr.push(i);

    setArr(newArr);
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

  const lineDisplay = () => {
    if (selectedRouter === 0) {
      return arr.map((elem1) =>
        arr.map((elem2) =>
          elem1 > elem2 ? (
            <Line
              zIndex={0}
              key={elem1 * arr.length + elem2}
              borderColor='#0275d8'
              borderWidth={3}
              x0={xCoord(elem2 / arr.length, 0.7)}
              y0={yCoord(elem2 / arr.length, 0.7)}
              x1={xCoord(elem1 / arr.length, 0.7)}
              y1={yCoord(elem1 / arr.length, 0.7)}
            />
          ) : (
            ""
          )
        )
      );
    } else if (linkView === "connections") {
      return arr.map((elem) => (
        <Line
          zIndex={0}
          key={elem}
          borderColor='#df4759'
          borderWidth={3}
          x0={xCoord((Number(selectedRouter) - 1) / arr.length, 0.7)}
          y0={yCoord((Number(selectedRouter) - 1) / arr.length, 0.7)}
          x1={xCoord(elem / arr.length, 0.7)}
          y1={yCoord(elem / arr.length, 0.7)}
        />
      ));
    } else if (linkView === "paths") {
      return arr.map((elem) => (
        <Line
          zIndex={0}
          key={elem}
          borderColor='#0275d8'
          borderWidth={3}
          x0={xCoord((Number(selectedRouter) - 1) / arr.length, 0.7)}
          y0={yCoord((Number(selectedRouter) - 1) / arr.length, 0.7)}
          x1={xCoord(elem / arr.length, 0.7)}
          y1={yCoord(elem / arr.length, 0.7)}
        />
      ));
    }
  };

  return (
    <div ref={compRef} style={{ width: "100%", height: "100%" }}>
      <div>
        {arr.map((elem) => (
          <Router
            className={`router-${elem}`}
            key={elem}
            name={`${elem}`}
            selected={selectedRouter === elem}
            x={xCoord((elem - 1) / arr.length, 0.7)}
            y={yCoord((elem - 1) / arr.length, 0.7)}
          />
        ))}
      </div>
      <div>{lineDisplay()}</div>
    </div>
  );
};

RouterDisplay.propTypes = {
  topology: PropTypes.object.isRequired,
  settings: PropTypes.object.isRequired,
};

const stateToProps = (state) => ({
  topology: state.topology,
  settings: state.settings,
});

export default connect(stateToProps, {})(RouterDisplay);
