#pragma once

void royaltyshare::receive_wax_transfer(const name& from, const name& to, const eosio::asset& quantity, const std::string& memo) {

    if(from == get_self() || to != get_self()){
        return;
    }

    const name tkcontract = get_first_receiver();

    check(quantity.amount > 0 && quantity.amount <= MAX_ASSET_AMOUNT, "Invalid quantity."); 

    auto c_it = config_t.require_find(0, ERR_CONFIG_NOT_FOUND);

    uint64_t total_parts = 0;

    std::vector<RECEIVING_DAO> linked_daos = c_it->linked_daos;
    std::vector<RECEIVING_FARM> linked_farms = c_it->linked_farms;
    std::vector<RECEIVING_WALLET> linked_wallets = c_it->linked_wallets;

    if(linked_daos.size() > 0){
        for(auto d : linked_daos){
            total_parts += d.parts;
        }
    }

    if(linked_farms.size() > 0){
        for(auto f : linked_farms){
            total_parts += f.parts;
        }
    }    

    if(linked_wallets.size() > 0){
        for(auto w : linked_wallets){
            total_parts += w.parts;
        }
    }        

    if(total_parts == 0){
        return; //nothing to do, since we have no instructions on where to send funds
    }

    uint64_t quantity_amount_u64 = quantity.amount;

    if(total_parts > quantity_amount_u64){
        return; //1 share is larger than the smallest asset unit, so we can't do calculations
    }

    uint64_t amount_per_part = floor(quantity_amount_u64 / total_parts);

    for(auto d : linked_daos){
        uint64_t amount_to_send = amount_per_part * d.parts;
        announce_dao_deposit(d.dao_name, quantity.symbol, tkcontract);
        transfer_tokens(DAO_CONTRACT, asset(amount_to_send, quantity.symbol), tkcontract, DAO_DEPOSIT_MEMO(d.dao_name));
    }

    for(auto f : linked_farms){
        uint64_t amount_to_send = amount_per_part * f.parts;
        if(f.contract_name == FARM_CONTRACT_V1){
            transfer_tokens(FARM_CONTRACT_V1, asset(amount_to_send, quantity.symbol), tkcontract, FARM_DEPOSIT_MEMO(f.farm_name));
        }
        else if(f.contract_name == FARM_CONTRACT_V2){
            transfer_tokens(FARM_CONTRACT_V2, asset(amount_to_send, quantity.symbol), tkcontract, FARM_DEPOSIT_MEMO(f.farm_name));
        }        
    }  

    for(auto w : linked_wallets){
        uint64_t amount_to_send = amount_per_part * w.parts;
        if(w.wax_address != get_self()){
            transfer_tokens(w.wax_address, asset(amount_to_send, quantity.symbol), tkcontract, w.memo);
        }
    }        

} 

/** 
 *  receive_nft_transfer
 *  this is required so that the catchall *::transfer does not throw an error when
 *  nfts are sent to this contract
 */

void royaltyshare::receive_nft_transfer(const name& from, const name& to, const std::vector<uint64_t>& asset_ids, const std::string& memo) {
    return;
} 


void royaltyshare::receive_any_transfer(const name& from, const name& to, const eosio::asset& quantity, const std::string& memo) {
    if(from == get_self() || to != get_self()){
        return;
    }

    const name tkcontract = get_first_receiver();

    check(quantity.amount > 0 && quantity.amount <= MAX_ASSET_AMOUNT, "Invalid quantity."); 

    /** We aren't accounting for farm shares here, UNLESS there is a rule set for this token
     *  because we only deal with farms where WAX is the reward token. 
     *  That is handled in receive_wax_transfer.
     */

    auto c_it = config_t.require_find(0, ERR_CONFIG_NOT_FOUND);

    // allow_all_tokens needs to be enabled in the config, or else we just ignore this transfer

    if(c_it->allow_all_tokens == 0){
        return;
    }

    uint64_t total_parts = 0;

    auto rules_secondary = rules_t.get_index<"symcontract"_n>();
    const uint128_t symbol_contract_combo = mix64to128(quantity.symbol.code().raw(), tkcontract.value);
    auto rules_itr = rules_secondary.find(symbol_contract_combo);

    std::vector<RECEIVING_DAO> linked_daos {};
    std::vector<RECEIVING_WALLET> linked_wallets {};
    std::vector<RECEIVING_FARM> linked_farms {};

    if(rules_itr != rules_secondary.end()){
        linked_daos = rules_itr->linked_daos;
        linked_wallets = rules_itr->linked_wallets;
        linked_farms = rules_itr->linked_farms;
    }

    else{
        linked_daos = c_it->linked_daos;
        linked_wallets = c_it->linked_wallets;    
    }

    if(linked_daos.size() > 0){
        for(auto d : linked_daos){
            total_parts += d.parts;
        }
    }

    if(linked_wallets.size() > 0){
        for(auto w : linked_wallets){
            total_parts += w.parts;
        }
    }  

    if(linked_farms.size() > 0){
        for(auto f : linked_farms){
            total_parts += f.parts;
        }
    }           

    if(total_parts == 0){
        return; //nothing to do, since we have no instructions on where to send funds
    }

    uint64_t quantity_amount_u64 = quantity.amount;

    if(total_parts > quantity_amount_u64){
        return; //1 share is larger than the smallest asset unit, so we can't do calculations
    }

    uint64_t amount_per_part = floor(quantity_amount_u64 / total_parts);

    for(auto d : linked_daos){
        uint64_t amount_to_send = amount_per_part * d.parts;
        announce_dao_deposit(d.dao_name, quantity.symbol, tkcontract);
        transfer_tokens(DAO_CONTRACT, asset(amount_to_send, quantity.symbol), tkcontract, DAO_DEPOSIT_MEMO(d.dao_name));
    }

    for(auto w : linked_wallets){
        uint64_t amount_to_send = amount_per_part * w.parts;
        if(w.wax_address != get_self()){
            transfer_tokens(w.wax_address, asset(amount_to_send, quantity.symbol), tkcontract, w.memo);
        }
    } 

    for(auto f : linked_farms){
        uint64_t amount_to_send = amount_per_part * f.parts;
        if(f.contract_name == FARM_CONTRACT_V1){
            transfer_tokens(FARM_CONTRACT_V1, asset(amount_to_send, quantity.symbol), tkcontract, FARM_DEPOSIT_MEMO(f.farm_name));
        }
        else if(f.contract_name == FARM_CONTRACT_V2){
            transfer_tokens(FARM_CONTRACT_V2, asset(amount_to_send, quantity.symbol), tkcontract, FARM_DEPOSIT_MEMO(f.farm_name));
        }  
    }          

} 
