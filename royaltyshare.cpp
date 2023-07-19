#pragma once

#include "royaltyshare.hpp"
#include "functions.cpp"
#include "on_notify.cpp"

ACTION royaltyshare::addauthacct(const name& account_to_add)
{
	require_auth(get_self());

	if(!is_account(account_to_add)){
		check(false, "account_to_add does not exist");
	}

	auto itr = config_t.require_find(0, ERR_CONFIG_NOT_FOUND);

	config_t.modify(itr, get_self(), [&](auto &_config){
		_config.authorized_accounts.push_back(account_to_add);
	});

}

ACTION royaltyshare::allowalltokn(const name& user, const uint8_t& is_allowed)
{
	require_auth(user);
	if(user != get_self()){
		check_username(user);
	}


	if(is_allowed != 0 && is_allowed != 1){
		check(false, "put 0 to disable, 1 to enable");
	}

	auto itr = config_t.require_find(0, ERR_CONFIG_NOT_FOUND);

	config_t.modify(itr, get_self(), [&](auto &_config){
		_config.allow_all_tokens = is_allowed;
	});

}

ACTION royaltyshare::claimroyalty()
{
	auto config_itr = config_t.require_find(0, ERR_CONFIG_NOT_FOUND);

	uint64_t seconds_between_claims = config_itr->hours_between_claims * 3600;
	uint64_t next_claim = config_itr->last_claim + seconds_between_claims;

	if(next_claim > now()){
		check(false, "Next claim is not until " + to_string(next_claim));
	}

	auto atomic_itr = balances.require_find(get_self().value, "No balance object found");

	for(eosio::asset bal : atomic_itr->quantities){
		if(bal.amount > 0 && bal.symbol == WAX_SYMBOL){
			claim_from_atomicmarket(bal);

			config_t.modify(config_itr, get_self(), [&](auto &_config){
				_config.last_claim = now();
			});
		}
	}

}

ACTION royaltyshare::initconfig()
{
	auto itr = config_t.find(0);

	if(itr != config_t.end()){
		return;
	}

	config_t.emplace(get_self(), [&](auto &_config){
		_config.ID = 0;
		_config.hours_between_claims = 24;
	});

}

ACTION royaltyshare::insertdao(const name& user, const name& dao_name, const uint64_t& parts)
{
	require_auth(user);
	if(user != get_self()){
		check_username(user);
	}

	auto it = daos_t.require_find(dao_name.value, ERR_DAO_DOESNT_EXIST);

	if(parts <= 0 || parts > 100){
		check(false, "parts must be 1-100");
	}

	auto c_it = config_t.require_find(0, ERR_CONFIG_NOT_FOUND);

	std::vector<RECEIVING_DAO> linked_daos = c_it->linked_daos;

	bool found = false;

	for(auto& dao : linked_daos) {
	    if(dao.dao_name == dao_name) {
	        dao.parts = parts;
	        found = true;
	        break;
	    }
	}

	if (!found) {
	    linked_daos.push_back(RECEIVING_DAO{dao_name, parts});
	}

	config_t.modify(c_it, get_self(), [&](auto& row) {
	    row.linked_daos = linked_daos;
	});

}

ACTION royaltyshare::insertfarm(const name& user, const name& farm_name, const uint64_t& parts)
{
	require_auth(user);
	if(user != get_self()){
		check_username(user);
	}

	auto it = farms_t.require_find(farm_name.value, ERR_FARM_DOESNT_EXIST);

	if(it->poolsize.symbol != WAX_SYMBOL || it->contract != WAX_CONTRACT){
		check(false, "cannot link farm unless reward token is WAX");
	}

	if(parts <= 0 || parts > 100){
		check(false, "parts must be 1-100");
	}

	auto c_it = config_t.require_find(0, ERR_CONFIG_NOT_FOUND);

	std::vector<RECEIVING_FARM> linked_farms = c_it->linked_farms;

	bool found = false;

	for(auto& farm : linked_farms) {
	    if(farm.farm_name == farm_name) {
	        farm.parts = parts;
	        found = true;
	        break;
	    }
	}

	if (!found) {
	    linked_farms.push_back(RECEIVING_FARM{farm_name, parts});
	}

	config_t.modify(c_it, get_self(), [&](auto& row) {
	    row.linked_farms = linked_farms;
	});

}

ACTION royaltyshare::insertwallet(const name& user, const name& wax_address, const uint64_t& parts,
	const std::string& memo)
{
	require_auth(user);
	if(user != get_self()){
		check_username(user);
	}

	if(!is_account(wax_address)){
		check(false, (wax_address.to_string() + " is not a wax address").c_str());
	}

	check( memo.size() <= 256, "memo has more than 256 bytes" );

	if(parts <= 0 || parts > 100){
		check(false, "parts must be 1-100");
	}

	auto c_it = config_t.require_find(0, ERR_CONFIG_NOT_FOUND);

	std::vector<RECEIVING_WALLET> linked_wallets = c_it->linked_wallets;

	bool found = false;

	for(auto& wallet : linked_wallets) {
	    if(wallet.wax_address == wax_address) {
	        wallet.parts = parts;
	        wallet.memo = memo;
	        found = true;
	        break;
	    }
	}

	if (!found) {
	    linked_wallets.push_back(RECEIVING_WALLET{wax_address, parts, memo});
	}

	config_t.modify(c_it, get_self(), [&](auto& row) {
	    row.linked_wallets = linked_wallets;
	});

}

ACTION royaltyshare::rmvauthacct(const name& account_to_remove)
{
	require_auth(get_self());

	auto itr = config_t.require_find(0, ERR_CONFIG_NOT_FOUND);

	std::vector<eosio::name> existingAccounts = itr->authorized_accounts;

	existingAccounts.erase(std::remove(existingAccounts.begin(), existingAccounts.end(), account_to_remove), existingAccounts.end());

	config_t.modify(itr, get_self(), [&](auto &_config){
		_config.authorized_accounts = existingAccounts;
	});

}

ACTION royaltyshare::removedao(const name& user, const name& dao_name)
{
	require_auth(user);
	if(user != get_self()){
		check_username(user);
	}

	auto c_it = config_t.require_find(0, ERR_CONFIG_NOT_FOUND);

	std::vector<RECEIVING_DAO> linked_daos = c_it->linked_daos;

	linked_daos.erase(
	    std::remove_if(
	        linked_daos.begin(), 
	        linked_daos.end(), 
	        [&](const RECEIVING_DAO& dao){ return dao.dao_name == dao_name; }),
	    linked_daos.end());

	config_t.modify(c_it, get_self(), [&](auto& row) {
	    row.linked_daos = linked_daos;
	});
}

ACTION royaltyshare::removefarm(const name& user, const name& farm_name)
{
	require_auth(user);
	if(user != get_self()){
		check_username(user);
	}

	auto c_it = config_t.require_find(0, ERR_CONFIG_NOT_FOUND);

	std::vector<RECEIVING_FARM> linked_farms = c_it->linked_farms;

	linked_farms.erase(
	    std::remove_if(
	        linked_farms.begin(), 
	        linked_farms.end(), 
	        [&](const RECEIVING_FARM& farm){ return farm.farm_name == farm_name; }),
	    linked_farms.end());

	config_t.modify(c_it, get_self(), [&](auto& row) {
	    row.linked_farms = linked_farms;
	});
}


ACTION royaltyshare::removewallet(const name& user, const name& wax_address)
{
	require_auth(user);
	if(user != get_self()){
		check_username(user);
	}

	auto c_it = config_t.require_find(0, ERR_CONFIG_NOT_FOUND);

	std::vector<RECEIVING_WALLET> linked_wallets = c_it->linked_wallets;

	linked_wallets.erase(
	    std::remove_if(
	        linked_wallets.begin(), 
	        linked_wallets.end(), 
	        [&](const RECEIVING_WALLET& wallet){ return wallet.wax_address == wax_address; }),
	    linked_wallets.end());

	config_t.modify(c_it, get_self(), [&](auto& row) {
	    row.linked_wallets = linked_wallets;
	});
}


ACTION royaltyshare::updatehours(const name& your_wallet, const uint64_t& hours_between_claims)
{
	require_auth(your_wallet);
	check_username(your_wallet);

	if(hours_between_claims <= 0 || hours_between_claims > MAX_HOURS_BETWEEN_CLAIMS){
		check(false, "hours_between_claims is an invalid number");
	}

	auto itr = config_t.require_find(0, ERR_CONFIG_NOT_FOUND);

	config_t.modify(itr, get_self(), [&](auto &_config){
		_config.hours_between_claims = hours_between_claims;
	});

}