#pragma once

struct COLLECTION_AND_SCHEMA {
	eosio::name 	collection_name;
	eosio::name  	schema_name;
};

struct STAKEABLE_COLLECTION {
	eosio::name  		collection_name;
	uint8_t  			is_authorized; 
};

struct RECEIVING_DAO {
	eosio::name  		dao_name;
	uint64_t  			parts; 
};

struct RECEIVING_FARM {
	eosio::name  		farm_name;
	uint64_t  			parts; 
};

struct RECEIVING_WALLET {
	eosio::name  		wax_address;
	uint64_t  			parts; 
	std::string  		memo;
};