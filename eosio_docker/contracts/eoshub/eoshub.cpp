#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

#include "../eosio.token/eosio.token.hpp"

using namespace eosio;

const symbol hub_symbol = symbol("EOSHUB", 4);
const name hub_name = name("eoshub");

// for v1 we're using this basic calculation, but it is incorrect
const float inflation_rate_per_year = .2f;
const float seconds_per_year = 31557600;
// this should be based on a compounding interest formula to figure out the spot inflation
// const float inflation_rate_per_second = inflation_rate_per_year * seconds_per_year;

// for the hackathon demo we'll set this to something that demonstrates earnings
const float inflation_rate_per_second = .001f;

class [[eosio::contract]] eoshub : public eosio::contract {
    private:            
    struct [[eosio::table("services")]] service {
        name owner;

        std::string name;
        std::string description;
        std::string url;

        std::string unit;
        float unit_price; // hub per unit

        auto primary_key() const { return owner.value; }
    };
    typedef eosio::multi_index<name("services"), service> services_index;


    struct [[eosio::table("subscribers")]] subscription {
        uint64_t id;
        name client;
        name service;
        asset amount_staked;

        auto primary_key() const { return id; }
        uint64_t by_client() const {return client.value; }
        uint64_t by_service() const { return service.value; }
    };

    typedef eosio::multi_index<name("subscribers"), subscription, 
            indexed_by <name("byclient"), const_mem_fun<subscription, uint64_t, &subscription::by_client>>,
            indexed_by <name("byservice"), const_mem_fun<subscription, uint64_t, &subscription::by_service>>
    > subscription_index;

    struct [[eosio::table("accounts")]] account {
        name owner;

        asset balance;
        asset staked_balance;

        uint64_t last_collection_time;
        auto primary_key() const { return owner.value; }


        static inline auto stake_spent(name scope, name account) {
            auto spent = asset(0, hub_symbol);

            subscription_index subscribers(scope, scope.value);
            auto byclient = subscribers.get_index<name("byclient")>();

            for (auto itr = byclient.find(account.value); itr != byclient.end(); itr++) {
                spent += itr->amount_staked;
            }
            return spent;
        }

        static inline auto available_stake(name scope, name account) {
            accounts_index accounts(scope, scope.value);
            auto itr = accounts.find(account.value);

            return itr->staked_balance - stake_spent(scope, account);
        }

        static inline auto earning_stake(name scope, name account) {             
            auto income = asset(0, hub_symbol);

            subscription_index subscribers(scope, scope.value);
            auto byservice = subscribers.get_index<name("byservice")>();

            for (auto itr = byservice.find(account.value); itr != byservice.end(); itr++) {
                income += itr->amount_staked;
            }

            return available_stake(scope,account) + income;         
        } 
    };
    typedef eosio::multi_index<name("accounts"), account> accounts_index;


    public:
    using contract::contract;

    // regservice registers an eosaccount service listing (metadata about the service etc)
    [[eosio::action]] void regservice(name owner, std::string name, std::string description, std::string url, std::string unit, float unit_price) {
        require_auth(owner);

        services_index services(_self, _self.value);
        services.emplace(owner, [&]( auto& svc ) {
            svc.owner = owner;
            svc.name = name;
            svc.description = description;
            svc.url = url;
            svc.unit = unit;
            svc.unit_price = unit_price;            
        });
    }

    // stake stakes a balance in eoshub allowing it to be used for api services
    [[eosio::action]] void stake(name owner, asset stakeAmount) {
        require_auth(owner);

        accounts_index accounts(_self,  _self.value);
        auto itr = accounts.find(owner.value);

        eosio_assert(itr != accounts.end(), "account not found");
        eosio_assert(stakeAmount.symbol == itr->balance.symbol, "incorrect symbol");
        eosio_assert(stakeAmount.is_valid(), "stakeAmount is not valid");
        eosio_assert(stakeAmount.amount > 0 , "stakeAmount must be positive");        
        eosio_assert(stakeAmount.amount <= itr->balance.amount, "insufficient funds");

        accounts.modify(itr, _self, [&](auto &a) {
            a.balance -= stakeAmount;
            a.staked_balance += stakeAmount;
        });
    }

    // unstake unstakes balances allowing them to be withdrawn from the contract
    [[eosio::action]] void unstake(name owner, asset unstakeAmount) {
        require_auth(owner);

        accounts_index accounts(_self,  _self.value);
        auto itr = accounts.find(owner.value);

        eosio_assert(itr != accounts.end(), "account not found");

        eosio_assert(unstakeAmount.symbol == itr->staked_balance.symbol, "incorrect symbol");
        eosio_assert(unstakeAmount.is_valid(), "unstakeAmount is not valid");
        eosio_assert(unstakeAmount.amount > 0 , "unstakeAmount must be positive");
        eosio_assert(unstakeAmount.amount <= itr->staked_balance.amount, "insufficient funds");

        //todo: force 1 month timeout

        accounts.modify(itr, _self, [&](auto &a) {
            a.balance += unstakeAmount;
            a.staked_balance -= unstakeAmount;
        });
    }

    // regapikey stakes a certain amount of eoshub with the service tied to an EOS public key
    [[eosio::action]] void subservice(name client, name service, asset delegateAmount) {
        require_auth(client);

        //todo trigger inline action that settles up rewards for both client/service up to this point

        accounts_index accounts(_self,  _self.value);
        auto clientitr = accounts.find(client.value);
        eosio_assert(clientitr != accounts.end(), "account not found");
        
        auto svcitr = accounts.find(service.value);
        eosio_assert(svcitr != accounts.end(), "service not found");

        //todo use eosio custom permission or actually validate ecc

        print("unstakedBalance: ", clientitr->balance, "\n");
        print("stakedBalance: ", clientitr->staked_balance, "\n");
        print("availableStake: ", account::available_stake(_self, client), "\n");
        print("stakeSpent: ", account::stake_spent(_self, client), "\n");
        print("earningStake: ", account::earning_stake(_self, client), "\n");

        eosio_assert(delegateAmount.symbol == clientitr->staked_balance.symbol, "incorrect symbol");
        eosio_assert(delegateAmount.is_valid(), "delegateAmount is not valid");
        eosio_assert(delegateAmount.amount > 0 , "delegateAmount must be positive");
        eosio_assert(delegateAmount.amount <= account::available_stake(_self, client).amount, "insufficient funds");

        // add registration to the table
        subscription_index registrations(_self, _self.value);
        registrations.emplace(_self, [&](auto &r) {
            r.id = registrations.available_primary_key();
            r.client = client;
            r.service = service;
            r.amount_staked = delegateAmount;
        });
    }

    // unregapikey unstakes an amount of eoshub with the service
    [[eosio::action]] void unsubservice(name client, name service, uint64_t registrationId) {
        require_auth(client);

        //todo trigger inline action that settles up rewards for both client/service up to this point

        accounts_index accounts(_self,  _self.value);
        auto clientitr = accounts.find(client.value);
        eosio_assert(clientitr != accounts.end(), "account not found");
        
        auto svcitr = accounts.find(service.value);
        eosio_assert(svcitr != accounts.end(), "service not found");

        //todo use eosio custom permission or actually validate ecc
        // eosio_assert(key != "", "key must be a valid eos public key");

        subscription_index registrations(_self, _self.value);

        auto itr = registrations.find(registrationId);
        eosio_assert( itr != registrations.end(), "registration not found");
        registrations.erase(itr);
    }

    // collectreward collects earnings from shares since account.last_collected_time
    [[eosio::action]] void collectreward(name user) {
        require_auth(user);
        accounts_index accounts(_self,  _self.value);

        auto itr = accounts.find(user.value);
        eosio_assert(itr != accounts.end(), "account not found");

        auto hub_token_supply = token::get_supply(name("eosio.token"), hub_symbol.code());
        float earning_ratio = float(account::earning_stake(_self, user).amount) / float(hub_token_supply.amount);
        auto earning_period = now() - itr->last_collection_time; //number of seconds since last collection
        auto total_earnings_this_period = (hub_token_supply.amount * inflation_rate_per_second) * earning_period;

        auto shares_earned = total_earnings_this_period * earning_ratio;

        print("supply: ", hub_token_supply, "\n");
        print("earningStake: ", account::earning_stake(_self, user), "\n");
        print("earningRatio: ", earning_ratio, "\n");
        print("earningPeriod: ", earning_period,"\n");
        print("totalEarningsThisPeriod: ", total_earnings_this_period,"\n");
        print("sharesEarned: ", shares_earned,"\n");

        eosio_assert(shares_earned > 0, "no shares earned");

        auto asset_earned = asset(shares_earned * 1000, hub_symbol);

        // issue shares w/ inline action
        action{
            permission_level{name("eosio"), name("active")},
            name("eosio.token"),
            name("issue"),
            std::tuple<name, asset, std::string>{user, asset_earned, "collect rewards " + std::to_string(now())}
        }.send();

    }

    // withdraw withdraws an amount of eoshub (from the unstaked balance) to a given account
    // cleos set account permission eoshub active '{"threshold": 1,"keys": [{"key": "ACTIVE PUBKEY","weight": 1}],"accounts": [{"permission":{"actor":"eoshub","permission":"eosio.code"},"weight":1}]}' owner -p eoshub@owner
    [[eosio::action]] void withdraw(name to, asset withdrawlAmount) { 
        require_auth(to);
        accounts_index accounts(_self,  _self.value);

        auto itr = accounts.find(to.value);
        eosio_assert(itr != accounts.end(), "account not found");

        // sanity checks on the withdrawl
        eosio_assert(withdrawlAmount.symbol == hub_symbol, "withdrawlAmount has incorrect symbol");
        eosio_assert(withdrawlAmount.is_valid(), "withdrawlAmount is not valid");
        eosio_assert(withdrawlAmount.amount > 0, "withdrawlAmount must be positive");

        // finally check that we have the funds available to withdraw
        eosio_assert(withdrawlAmount.amount <= itr->balance.amount, "insufficient unstaked funds");                

        // use inline action to send withdrawlAmount via the eosio.token contract
        action{
            permission_level{_self, name("active")},
            name("eosio.token"),
            name("transfer"),
            std::tuple<name, name, asset, std::string>{name("eoshub"), to, withdrawlAmount, "withdrawl from eoshub"}
        }.send();

        // modified the stored balance on our ledger
        accounts.modify(itr, to, [&](auto &a) {
            a.balance -= withdrawlAmount;
        });
    }

    // deposithub notification _from_ the eoshub.token contract
    [[eosio::action]] void deposithub( name from, name to, asset quantity, std::string memo ) {
        if(from == _self || to != _self)
            return;

        eosio_assert(quantity.symbol == hub_symbol, "incorrect symbol");
        eosio_assert(quantity.is_valid(), "quantity is not valid");
        eosio_assert(quantity.amount > 0, "amount must be positive");

        accounts_index accounts(_self, _self.value);
        auto itr = accounts.find(from.value);

        // We don't currently have an account for this user, lets start one
        if (itr == accounts.end()) {
            accounts.emplace(_self, [&](auto &a) {
                a.owner = from;
                a.balance = quantity;
                a.staked_balance = asset(0, hub_symbol);
                a.last_collection_time = now();
            });
            return;
        }

        // This account does exist, lets add its balance to our records
        eosio_assert(itr != accounts.end(), "account doesn't exist");
        accounts.modify(itr, _self, [&](auto &a){
            a.balance += quantity;
        });
    }


};

// Custom Dispatcher to handle deposit notifications into the token contract
extern "C" {
    void apply(uint64_t receiver, uint64_t code, uint64_t action) {
        if(code==receiver)
        {
            switch(action)
            {
                EOSIO_DISPATCH_HELPER(eoshub, (regservice)(stake)(unstake)(subservice)(unsubservice)(collectreward)(withdraw) )
            }
        }
        else if(code==name("eosio.token").value && action==name("transfer").value) {
            execute_action( name(receiver), name(code), &eoshub::deposithub );
        }
    }
};


