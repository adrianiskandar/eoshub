.EXPORT_ALL_VARIABLES:

CLEOS := docker exec eosio_notechain_container /opt/eosio/bin/cleos -u http://localhost:8888

all:
	mkdir -p build/eoshub/
	eosio-cpp -o build/eoshub/eoshub.wasm eoshub/eoshub.cpp -abigen --contract eoshub

setupkeys:
	$$CLEOS wallet import -n notechainwal --private-key 5KFyaxQW8L6uXFB6wSgC44EsAbzC7ideyhhQ68tiYfdKQp69xKo || true
	$$CLEOS wallet import -n notechainwal --private-key 5K7mtrinTFrVTduSxizUc5hjXJEtTjVTsqSHeBHes1Viep86FP5 || true
	$$CLEOS wallet import -n notechainwal --private-key 5KLqT1UFxVnKRWkjvhFur4sECrPhciuUqsYRihc1p9rxhXQMZBg || true
	
deploy: setupkeys
	$$CLEOS set account permission eoshub active '{"threshold": 1,"keys": [{"key": "EOS8Du668rSVDE3KkmhwKkmAyxdBd73B51FKE7SjkKe5YERBULMrw","weight": 1}],"accounts": [{"permission":{"actor":"eoshub","permission":"eosio.code"},"weight":1}]}' owner -p eoshub@owner || true
	$$CLEOS set account permission eosio active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor":"eoshub","permission":"eosio.code"},"weight":1}]}' owner -p eosio@owner || true
	$$CLEOS set contract eoshub /opt/eosio/bin/contracts/build/eoshub

test-deposit:
	echo "Depositing 5 EOSHUB into contract"
	$$CLEOS transfer useraaaaaaaa eoshub "5 EOSHUB" "test"
	$$CLEOS transfer useraaaaaaab eoshub "5 EOSHUB" "test"

	echo "Showing accounts"
	$$CLEOS get currency balance eosio.token useraaaaaaaa EOSHUB
	$$CLEOS get table eoshub eoshub accounts	
	$$CLEOS push action eoshub withdraw '["useraaaaaaaa","5.0000 EOSHUB"]' -p useraaaaaaaa@owner

	echo "Showing accounts"
	$$CLEOS get currency balance eosio.token useraaaaaaaa EOSHUB
	$$CLEOS get table eoshub eoshub accounts

test-subscribe:
	echo "useraaaaaaaa subscribes to useraaaaaaab w/ 5 EOSHUB of stake"
	$$CLEOS push action eoshub regapikey '['useraaaaaaaa','useraaaaaaab',"5.0000 EOSHUB"]' -p useraaaaaaaa@owner
	$$CLEOS get table eoshub eoshub accounts
	$$CLEOS get table eoshub eoshub subscribers

test-collect:
	echo "useraaaaaaab balance before collect"
	$$CLEOS get currency balance eosio.token useraaaaaaab EOSHUB
	$$CLEOS push action eoshub collectreward '['useraaaaaaab']' -p useraaaaaaab@active
	echo "useraaaaaaab balance after collect"
	$$CLEOS get currency balance eosio.token useraaaaaaab EOSHUB
