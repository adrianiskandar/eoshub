import React, { Component } from "react";
import { Link } from "react-router-dom";

import Image from "../img/logo.png";

class Navigation extends Component {
  render() {
    return (
      <div className="navigation" style={{paddingLeft: '20px'}}>
        <Link to="/">
          <img src={Image} />
        </Link>
        <div className="menu">
          <nav>
            <input type="text" placeholder="Search Services" />
            <span>
              <Link to="/services">Services</Link>
            </span>
            <span>
              <Link to="/utilization">My Utilization</Link>
            </span>
            <span>
              <Link to="/developers">Developers</Link>
            </span>
            <div style={{ color: "white", float: "right", paddingRight: "30px" }}> Balance: <b>12400</b> EOSHUB</div>
          </nav>
        </div>
      </div>
    );
  }
}

export default Navigation;
