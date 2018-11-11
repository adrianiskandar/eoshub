import React, { Component } from "react";
import Navigation from "../Components/Navigation";
import UtilizationCard from "../Components/UtilizationCard";

import img from "../img/elasticsearch.jpg";
import security from "../img/sec.png";
import security2 from "../img/antivirus.png";
import analy from "../img/analy.png";
import chat from "../img/chat.png";
import chat2 from "../img/chat2.png";
import db from "../img/db.png";


class services extends Component {

  render() {
    return (
      <div class="utilization">
        <Navigation />
        <h1 style={{margin: "30px 20px 10px 30px"}}>My API usage</h1>
        <UtilizationCard img={img} title={"Elastic API"} desc={"API leveraging Elastic Search to provide a seach engine for decentralized applications"}></UtilizationCard>
        <UtilizationCard img={security2} title={"Security API"} desc={"provides a layer of security to verify tokens are valid"}></UtilizationCard>
        <UtilizationCard img={chat2} title={"Chat API"} desc={"provides a layer of security to verify tokens are valid"}></UtilizationCard>
      </div>
    );
  }
}

export default services;
