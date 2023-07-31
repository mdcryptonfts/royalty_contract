#pragma once

#include "structs.hpp"

//Tables on other contracts

struct balances_s {
    name           owner;
    vector <asset> quantities;

    uint64_t primary_key() const { return owner.value; };
};
typedef multi_index <name("balances"), balances_s> balances_t;


struct [[eosio::table]] daos {
	eosio::name 							daoname;
	eosio::name  							creator;
	uint64_t  							time_created;
	uint8_t  							status;
	double  							threshold;
	uint8_t								dao_type; 
	uint8_t  							proposer_type; 
	std::vector<eosio::name>  					authors;
	eosio::symbol							gov_token_symbol;
	eosio::name  							gov_token_contract;
	uint64_t  							hours_per_proposal;
	uint64_t  							minimum_weight;
	uint64_t  							minimum_votes;
	eosio::name  							gov_farm_name;
	eosio::asset  							proposal_cost;
	std::vector<COLLECTION_AND_SCHEMA>  				gov_schemas;

	uint64_t primary_key() const { return daoname.value; }
};
using daotable = eosio::multi_index<"daos"_n, daos
>;


struct [[eosio::table]] farms {
	uint64_t 								ID;	
	eosio::name 								farmname;
	eosio::name 								creator;
	std::string 								rewardtoken;
	eosio::name 								contract;
	eosio::name 								collection;
	eosio::name 								schema;
	eosio::asset 								poolsize;
	uint64_t 								mintime;
	uint64_t 								enddate;
	std::string 								logo;
	eosio::asset 								hourlyreward;
	uint64_t 								totalstaked;
	eosio::name 								farmtype;
	std::vector<uint64_t> 							templates;
	uint32_t 								template_limit;
	uint64_t 								lastupdate;
	eosio::name 								reward_type;
	uint64_t 								hourly_cap;
	std::vector<STAKEABLE_COLLECTION> 					collections;
	uint8_t  								max_bonus_pools;
	uint8_t  								allow_new_members;
	eosio::name  								cpu_payer;
	uint64_t 								time_created;
	uint8_t  								tier;


	uint64_t primary_key() const { return ID; }
	uint64_t secondarykey() const { return farmname.value; }
	uint64_t thirdkey() const { return creator.value; }
	uint64_t fourthkey() const { return collection.value; }
	uint64_t fifthkey() const { return schema.value; }

};
using farms_table = eosio::multi_index<"farms"_n, farms,
eosio::indexed_by<"farmname"_n, eosio::const_mem_fun<farms, uint64_t, &farms::secondarykey>>,
eosio::indexed_by<"creator"_n, eosio::const_mem_fun<farms, uint64_t, &farms::thirdkey>>,
eosio::indexed_by<"collection"_n, eosio::const_mem_fun<farms, uint64_t, &farms::fourthkey>>,
eosio::indexed_by<"schema"_n, eosio::const_mem_fun<farms, uint64_t, &farms::fifthkey>>
>;

struct [[eosio::table, eosio::contract("farms.waxdao")]] farms_v2 {
	eosio::name 						farmname;
	eosio::name 						creator;
	uint64_t 						time_created;
	uint64_t 						last_state_change;
	uint64_t  						expiration;
	uint8_t  						status;
	std::vector<REWARD_POOL>  				reward_pools;
	uint8_t  						farm_type; 
	std::vector<STAKEABLE_COLLECTION>  			collections;
	uint64_t  						total_staked;
	uint64_t  						id;
	FARM_PROFILE  						profile;
	SOCIAL_LINKS  						socials;
	uint64_t  						hours_between_payouts;

	uint64_t primary_key() const { return farmname.value; }
	uint64_t secondarykey() const { return creator.value; }
	uint64_t thirdkey() const { return id; }

};
using v2_farms_t = eosio::multi_index<"farmsv2"_n, farms_v2,
eosio::indexed_by<"creator"_n, eosio::const_mem_fun<farms_v2, uint64_t, &farms_v2::secondarykey>>,
eosio::indexed_by<"id"_n, eosio::const_mem_fun<farms_v2, uint64_t, &farms_v2::thirdkey>>
>;

struct [[eosio::table]] stat {
  asset    	supply;
  asset 	max_supply;
  name 		issuer;

  uint64_t primary_key()const { return supply.symbol.code().raw(); }
};
typedef eosio::multi_index< "stat"_n, stat > stat_table;


//Tables on this contract

struct [[eosio::table, eosio::contract(CONTRACT_NAME)]] config2 {
	uint64_t  			ID;
	std::vector<RECEIVING_FARM>  	linked_farms;
	std::vector<RECEIVING_DAO>  	linked_daos;
	std::vector<RECEIVING_WALLET>  	linked_wallets;
	uint64_t  			hours_between_claims;
	uint64_t  			last_claim;
	std::vector<eosio::name>  	authorized_accounts;
	uint8_t  			allow_all_tokens;

	uint64_t primary_key() const { return ID; }
};
using configtable = eosio::multi_index<"config2"_n, config2>;

struct [[eosio::table, eosio::contract(CONTRACT_NAME)]] rules {
	uint64_t  			ID;
	eosio::symbol  			token_symbol;
	eosio::name  			contract;
	std::vector<RECEIVING_FARM>  	linked_farms;
	std::vector<RECEIVING_DAO>  	linked_daos;
	std::vector<RECEIVING_WALLET>  	linked_wallets;

	uint64_t primary_key() const { return ID; }
	uint128_t second_key() const { return mix64to128(token_symbol.code().raw(), contract.value); }
};
using rules_table = eosio::multi_index<"rules"_n, rules,
eosio::indexed_by<"symcontract"_n, eosio::const_mem_fun<rules, uint128_t, &rules::second_key>>
>;
