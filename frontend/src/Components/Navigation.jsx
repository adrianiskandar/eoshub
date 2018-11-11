import React, { Component } from "react";
import { Link } from "react-router-dom";

import Image from "../img/logo.png";

class Navigation extends Component {
  render() {
    return (
      <div class="navigation" style={{paddingLeft: '20px'}}>
        <Link to="/">
          <img src={Image} />
        </Link>
        <div class="menu">
          <nav>
            <input type="text" placeholder="Search Services" />
            <span>
              <Link to="/services">Services</Link>
            </span>
            <span>
              <Link to="/developers">Developers</Link>
            </span>
            <span>
              <Link to="/usage">Usage</Link>
            </span>
            <div class="balance" style={{ color: "white", float: "right", paddingRight: "30px" }}> Balance: <b>12400</b> EOSHUB</div>
          </nav>
        </div>
      </div>
    );
  }
}

export default Navigation;
