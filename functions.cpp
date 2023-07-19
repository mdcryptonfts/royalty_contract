#pragma once

void royaltyshare::announce_dao_deposit(const name& dao_name, const symbol& token_symbol, const name& token_contract){
	action(permission_level{get_self(), "active"_n}, DAO_CONTRACT,"tokendeposit"_n,std::tuple{ get_self(), dao_name, token_symbol, token_contract}).send();
	return;
}

void royaltyshare::check_username(const name& user){
	auto itr = config_t.require_find(0, "Could not locate config");
	check(std::find(itr->authorized_accounts.begin(), itr->authorized_accounts.end(), user) != itr->authorized_accounts.end(), "You aren't authorized to perform this action");	
	return;
}

void royaltyshare::claim_from_atomicmarket(const eosio::asset& balance){
	action(
		permission_level{get_self(), "active"_n},
		ATOMICMARKET_CONTRACT,
		"withdraw"_n,
		std::tuple{ get_self(), balance }
	).send();
}

uint64_t royaltyshare::now() {
  return current_time_point().sec_since_epoch();
}

void royaltyshare::transfer_tokens(const name& user, const asset& amount_to_send, const name& contract, const std::string& memo){
	action(permission_level{get_self(), "active"_n}, contract,"transfer"_n,std::tuple{ get_self(), user, amount_to_send, memo}).send();
	return;
}