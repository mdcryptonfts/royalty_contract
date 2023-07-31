#pragma once
#define CONTRACT_NAME "royaltyshare"
#define mix64to128(a, b) (uint128_t(a) << 64 | uint128_t(b))

#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/symbol.hpp>
#include <eosio/action.hpp>
#include <string>
#include <eosio/crypto.hpp>
#include <eosio/transaction.hpp>
#include "../tables/atomic.hpp"
#include <cmath>
#include "tables.hpp"
#include "structs.hpp"
#include "constants.hpp"
#include <limits>
#include "../tables/atomicdata.hpp"

using namespace eosio;


CONTRACT royaltyshare : public contract {
	public:
		using contract::contract;
		royaltyshare(name receiver, name code, datastream<const char *> ds):contract(receiver, code, ds){}

		//Main Actions
		ACTION addauthacct(const name& account_to_add);
		ACTION addrule(const name& user, const symbol& token_symbol, const name& contract);
		ACTION allowalltokn(const name& user, const uint8_t& is_allowed);
		ACTION claimroyalty();
		ACTION initconfig();
		ACTION removedao(const name& user, const name& dao_name);
		ACTION removefarm(const name& user, const name& farm_name);
		ACTION removerule(const name& user, const symbol& token_symbol, const name& contract);
		ACTION removewallet(const name& user, const name& wax_address);
		ACTION rmvauthacct(const name& account_to_remove);
		ACTION ruledaormv(const name& user, const name& dao_name, const symbol& token_symbol, const name& contract);
		ACTION ruledaoupdt(const name& user, const name& dao_name, const uint64_t& parts, 
			const symbol& token_symbol, const name& contract);
		ACTION rulefarmrmv(const name& user, const name& farm_name, const symbol& token_symbol, const name& contract);
		ACTION rulefarmupdt(const name& user, const name& farm_contract, const name& farm_name, const uint64_t& parts,
			const symbol& token_symbol, const name& token_contract);
		ACTION rulewlltrmv(const name& user, const name& wax_address, const symbol& token_symbol, const name& contract);
		ACTION rulewlltupdt(const name& user, const name& wax_address, const uint64_t& parts,
			const std::string& memo, const symbol& token_symbol, const name& contract);
		ACTION updatehours(const name& your_wallet, const uint64_t& hours_between_claims);
		ACTION upsertdao(const name& user, const name& dao_name, const uint64_t& parts);
		ACTION upsertfarm(const name& user, const name& farm_contract, const name& farm_name, const uint64_t& parts);
		ACTION upsertwallet(const name& user, const name& wax_address, const uint64_t& parts,
			const std::string& memo);		

		//Notifications
		[[eosio::on_notify("eosio.token::transfer")]] void receive_wax_transfer(const name& from, const name& to, const eosio::asset& quantity, const std::string& memo);
		[[eosio::on_notify("atomicassets::transfer")]] void receive_nft_transfer(const name& from, const name& to, const std::vector<uint64_t>& asset_ids, const std::string& memo);
		[[eosio::on_notify("*::transfer")]] void receive_any_transfer(const name& from, const name& to, const eosio::asset& quantity, const std::string& memo);

	private:

		//Tables
		balances_t balances = balances_t(ATOMICMARKET_CONTRACT, ATOMICMARKET_CONTRACT.value);
		configtable config_t = configtable(get_self(), get_self().value);
		daotable daos_t = daotable(DAO_CONTRACT, DAO_CONTRACT.value);
		farms_table farms_t = farms_table(FARM_CONTRACT_V1, FARM_CONTRACT_V1.value);
		v2_farms_t farms_t_v2 = v2_farms_t(FARM_CONTRACT_V2, FARM_CONTRACT_V2.value);
		rules_table rules_t = rules_table(get_self(), get_self().value);


		//Functions
		void announce_dao_deposit(const name& dao_name, const symbol& token_symbol, const name& token_contract);
		void check_token_exists(const symbol& token_symbol, const name& contract);
		void check_username(const name& user);
		void claim_from_atomicmarket(const eosio::asset& balance);
		uint64_t now();
		void transfer_tokens(const name& user, const asset& amount_to_send, const name& contract, const std::string& memo);

};
