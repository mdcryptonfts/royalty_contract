#pragma once

struct COLLECTION_AND_SCHEMA {
	eosio::name 	collection_name;
	eosio::name  	schema_name;
};

struct STAKEABLE_COLLECTION {
	eosio::name  		collection_name;
	uint8_t  		is_authorized; 
};

struct RECEIVING_DAO {
	eosio::name  		dao_name;
	uint64_t  		parts; 
};

struct RECEIVING_FARM {
	eosio::name  		contract_name;
	eosio::name  		farm_name;
	uint64_t  		parts; 
};

struct RECEIVING_WALLET {
	eosio::name  		wax_address;
	uint64_t  		parts; 
	std::string  		memo;
};

struct REWARD_POOL {
	eosio::asset	total_funds;
	eosio::name  	contract;
	eosio::asset  	total_hourly_reward;
};

struct FARM_PROFILE {
	std::string 		avatar;
	std::string  		cover_image;
	std::string  		description; 
};

struct SOCIAL_LINKS {
	std::string 		website;
	std::string  		telegram;
	std::string  		discord; 
	std::string  		twitter; 
	std::string  		medium; 
	std::string  		youtube;
	std::string  		atomichub;
	std::string  		waxdao;
};
