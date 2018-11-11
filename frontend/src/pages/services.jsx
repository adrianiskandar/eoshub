import React, { Component } from "react";
import Navigation from "../Components/Navigation";
import ServiceCard from "../Components/ServiceCard";

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
      <div>
        <Navigation />
        <h1 style={{margin: "30px 20px 10px 30px"}}>Select which service to use:</h1>
        <ServiceCard img={img} title={"Elastic API"} desc={"API leveraging Elastic Search to provide a seach engine for decentralized applications"}></ServiceCard>
        <ServiceCard img={security2} title={"Security API"} desc={"provides a layer of security to verify tokens are valid"}></ServiceCard>
        <ServiceCard img={analy} title={"Analytics API"} desc={"Analytics for your API"}></ServiceCard>
        <ServiceCard img={chat} title={"Chat API"} desc={"API leveraging Elastic Search to provide a seach engine for decentralized applications"}></ServiceCard>
        <ServiceCard img={security} title={"Best Shield API"} desc={"Best security for API interaction"}></ServiceCard>
        <ServiceCard img={db} title={"SQL on EOS"} desc={"SQL for your API"}></ServiceCard>
        <ServiceCard img={img} title={"Elastic 2.0 API"} desc={"Elastic Search 2.0 to provide a seach engine for decentralized applications"}></ServiceCard>
        <ServiceCard img={chat2} title={"Chat API"} desc={"provides a layer of security to verify tokens are valid"}></ServiceCard>
      </div>
    );
  }
}

export default services;
