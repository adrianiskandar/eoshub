import React, { Component } from "react";
import img from "../img/elasticsearch.jpg";
import { Api, JsonRpc, RpcError, JsSignatureProvider } from 'eosjs'; // https://github.com/EOSIO/eosjs

import "rc-slider/assets/index.css";
import Tooltip from "rc-tooltip";
import Slider from "rc-slider";

const createSliderWithTooltip = Slider.createSliderWithTooltip;
const Range = createSliderWithTooltip(Slider.Range);
const Handle = Slider.Handle;

// eosio endpoint
const endpoint = "http://localhost:8888";
const account =  {"name":"useraaaaaaaa", "privateKey":"5K7mtrinTFrVTduSxizUc5hjXJEtTjVTsqSHeBHes1Viep86FP5", "publicKey":"EOS6kYgMTCh1iqpq9XGNQbEi8Q6k5GujefN9DSs55dcjVyFAq7B6b"}
  ;

class services extends Component {
  constructor() {
    super();
    this.state = {
      iniRequests: 50000,
      requests: 50000
    };
    this.setRequestsPerMinute = this.setRequestsPerMinute.bind(this);
    this.handle = this.handle.bind(this);
  }

  handle(props) {
    const { value, dragging, index, ...restProps } = props;
    if (this.state.requests != value) {
      this.setRequestsPerMinute(value);
    }
    return (
      <Tooltip
        prefixCls="rc-slider-tooltip"
        overlay={value}
        visible={dragging}
        placement="top"
        key={index}
      >
        <Handle value={value} {...restProps} />
      </Tooltip>
    );
  }

  setRequestsPerMinute(value) {
    this.setState({ requests: value });
  }

  async stakeToken(owner, stakeAmount){
    // eosjs function call: connect to the blockchain
    const rpc = new JsonRpc(endpoint);
    const signatureProvider = new JsSignatureProvider([account.privateKey]);
    const api = new Api({ rpc, signatureProvider, textDecoder: new TextDecoder(), textEncoder: new TextEncoder() });
    try {
      const result = await api.transact({
        actions: [{
          account: "notechainacc",
          name: "stake",
          authorization: [{
            actor: owner,
            permission: 'active',
          }],
          data: {
            owner,
            stakeAmount,
          },
        }]
      }, {
        blocksBehind: 3,
        expireSeconds: 30,
      });

      console.log(result);
    } catch (e) {
      console.log('Caught exception: ' + e);
      if (e instanceof RpcError) {
        console.log(JSON.stringify(e.json, null, 2));
      }
    }
  }
  
  render() {
    return (
      <div className="card">
        <img className="card-img-top" src={this.props.img} alt="Elastic Service" />
        <div className="card-body">
          <h5 className="card-title">{this.props.title}</h5>
          <p className="card-text">{this.props.desc} </p>
          <div className="slider">
            <p>Requests per Month: {this.state.requests}</p>
            <Slider
              min={1000}
              max={200000}
              defaultValue={
                10000 + Math.round((this.state.iniRequests * Math.random()) / 10000) * 10000
              }
              handle={this.handle}
            />
          </div>
          <a id="elastic" className="btn btn-primary" style={{ color: "white" }}>
            Stake {parseFloat(this.state.requests * 0.000001).toFixed(4)} EOSHUB tokens
          </a>
        </div>
      </div>
    );
  }
}

export default services;
