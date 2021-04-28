import { combineReducers } from "redux";
import settings from "./settings";
import tables from "./tables";
import topology from "./topology";

export default combineReducers({ settings, tables, topology });
