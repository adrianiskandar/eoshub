import React, { Component } from "react";
import { BrowserRouter, Switch, Route } from "react-router-dom";

// import { BrowserRouter as Router, Link } from "react-router-dom";
// import { Switch, Route } from "react-router";

import Homepage from "../pages/homepage";
import Services from "../pages/services";
import Utilization from "../pages/utilization";
const Developers = () => <h2>Developers</h2>;
const Enterprise = () => <h2>Enterprise</h2>;
// import Index from '../pages/index';

const AppRouter = () => (
  <BrowserRouter>
    <Switch>
      <Route path="/" exact component={Homepage} />
      <Route path="/developers" component={Developers} />
      <Route path="/services" component={Services} />
      <Route path="/utilization" component={Utilization} />
    </Switch>
  </BrowserRouter>
);

export default AppRouter;
