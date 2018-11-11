import React, { Component } from "react";
import Navigation from "../Components/Navigation";
import ServiceCard from "../Components/ServiceCard";

import img from "../img/usage.png";


class services extends Component {

  render() {
    return (
      <div>
        <Navigation />
        <h1 style={{margin: "30px 20px 10px 30px"}}>Your current usage:</h1>
        <img src={img} />
      </div>
    );
  }
}

export default services;
