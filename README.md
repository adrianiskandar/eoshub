# <img src="https://github.com/xnastasia/eoshub/blob/master/logoonwhite.png?raw=true" height="150">

EOS hub is an API marketplace with staking for EOS developers.

EOS Hub allows dApp developers to enter a new era of decentralized collaborative programming. The same staking model of incentive alignment between dApps and BPs can now be applied to the software layer. 

The SaaS market is estimated to reach $200B while the API economy as a whole is estimated reaching $2.2T in the coming two years. 
EOS hub multiplies efficiency and value by providing developers the opportunity to work **together** instead of against each other. With EOS hub developers can now keep a stake in the game instead of having their work enrich shareholders while the rest of the IT economy struggles with the same old competitive model based on spending.

## Smart contract

The smart contract can be found at [`eosio_docker/contracts/eoshub/eoshub.cpp`](https://github.com/eoshubco/eoshub/blob/master/eosio_docker/contracts/eoshub/eoshub.cpp) .

Functionalities:

### regservice
This registers a service listing to be shown on the website

### stake 
This stakes EOSHUB on your account balance within our smart contract.  Staked balances can be used to subscribe to API services.


### unstake
Unstake starts the unstaking process.  For the purposes of the hackathon this is instantaneous, but in practice it would be set to 1 month.


### subservice
This creates a subscription to a service with a given amount of your staked balance to spend.  

### unsubservice
Allows you to unsubscribe from a service (erases a single subscription record)

### collectreward

Collect Reward triggers a payout based on your proportional distribution of the reward.  We compute the annual inflation rate as inflation per second, and then compute the earnings since the last time you called collectreward (or triggered it via another action).


### withdraw 
withdraw your unstaked EOSHUB back to your account balance

### deposithub
Notification handler listening for transfers on the eosio.token contract.

# Use

After making an account and paying for service subscriptions, developers can install the API of their choice with one line of code:

```sh
eoshub install eoshub.co/company/bestapiever.something
```

Great!

