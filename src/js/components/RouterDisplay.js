import React, { useState, useEffect, useRef } from "react";
import PropTypes from "prop-types";

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

const RouterDisplay = () => {
  const arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
  const compRef = useRef(null);
  const winSize = useWindowSize();

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
  }, [winSize]);

  const xCoord = (grad) =>
    Math.round(
      0.7 * (dim.width / 2) * Math.cos(2 * Math.PI * grad) + dim.width / 2
    );

  const yCoord = (grad) =>
    Math.round(
      0.7 * (dim.height / 2) * Math.sin(2 * Math.PI * grad) + dim.height / 2
    );

  return (
    <div ref={compRef} style={{ width: "100%", height: "100%" }}>
      {arr.map((elem) => (
        <Router
          key={elem}
          name={`${elem}`}
          x={xCoord((elem - 1) / arr.length)}
          y={yCoord((elem - 1) / arr.length)}
        />
      ))}
    </div>
  );
};

RouterDisplay.propTypes = {};

export default RouterDisplay;
