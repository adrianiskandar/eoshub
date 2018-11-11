import React, { Component } from "react";
import img from "../img/elasticsearch.jpg";
import { Api, JsonRpc, RpcError, JsSignatureProvider } from 'eosjs'; // https://github.com/EOSIO/eosjs

import "rc-slider/assets/index.css";
import Tooltip from "rc-tooltip";
import Slider from "rc-slider";
import Gauge from 'react-svg-gauge';
// import Gauge from 'react-radial-gauge';

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
        <div className="card-body">
          <h5 className="card-title">{this.props.title}</h5>
          <div className="slider">
            <p>My utilization: {this.state.requests}</p>
            <div>
            <Gauge value={ Math.round(Math.random()*100) } width={230} height={180} label="" />
            </div>
          </div>
        </div>
      </div>
    );
  }
}

export default services;
