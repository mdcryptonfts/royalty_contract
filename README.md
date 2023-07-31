# Royalty Contract Documentation

## What is this?

This is a contract that will automatically split up any funds received by your account, and send those split funds to whatever accounts that you choose. 

You can use this to automatically deposit funds to NFT farms, exchanges, DAO treasuries, random wax wallets, whatever you want. You can even include custom memos when you send funds out of this contract.

### Deploy this contract now on [WaxDAO.io](https://waxdao.io/royalty-contract)

## Before you bother with this contract, there are a few things you should be aware of.

- If you are trying to claim royalties from a collection where you do **not** own the private keys to the author wallet, then this contract can not claim royalties from your collection. However, it can still split any payments that it receives, so it's still very useful for automatically sharing payments, etc. **Breaking news: [MyCloudWallet now allows you to take full control of your account](https://wax-io.medium.com/unleash-the-power-of-flexibility-and-ownership-with-my-cloud-wallets-new-features-1275abdc1126), so you can use this contract even if your collection was created with a .wam account**

- You will need ~1,230,000 bytes of RAM to store this contract on your account (about 490 WAX worth, at the time of this writing. Check [waxblock](https://waxblock.io/wallet/ram) for up to date RAM prices.)

- You will need to add the **eosio.code** permission to your account, in order for this contract to work. I will include instructions below.

- ~~If you are planning on sending royalties to a WaxDAO NFT farm, the farm's reward token must be **WAX**. You will not be able to link your WaxDAO NFT farm to this contract unless: the farm exists, and the reward token is **WAX** (precision is **8**, contract is **eosio.token**).~~ **Update: token rules have now been added to the contract, so you can send ANY token to an NFT farm, not just the WAX token**

- The atomicmarket contract stores **all** of your claimable WAX in **1 single balance**. This is very important. If you have multiple schemas under 1 collection, all of the royalties will go into this 1 balance. If you make someone a buy offer from the same wallet that owns the collection; and then that buy offer gets rejected, that WAX will also go into this same balance. This contract will not differentiate between what "came from secondary" and what is "your own WAX". Keep this in mind when setting up your collection etc.

- There is a setting in this contract's configuration that allows you to choose whether or not to process transfers for **other tokens that are not WAX**.

- You can also send funds to DAO treasuries, if you want to send them to a v2 DAO on WaxDAO.

- You can also send funds to **any wax address, and you can even provide a custom memo**. One example use case for this: if you create a drop on any NFT market, you can set this contract as the receiver. This contract will then split up funds between any farms, DAOs, or even regular WAX wallets that you choose. A lot of you have asked "Can I send funds from my Nefty or Hive drop, directly to my NFT farm?"... Well, with this contract, yes.. you can. 

## Instructions


1. The very first thing you need to do when you upload the contract to your account, is use the **initconfig** action (go to your account on waxblock, click the "Contract Actions" tab, then click **initconfig**). 

	This action initializes the **config2** table with the default settings. If you want to adjust these settings, you can use the actions on this contract, which I will outline below.

	*optional: add authorized_users to the table using the master account (self), via the addauthacct action.*

2. Next step is to add any farms and/or v2 DAOs to the contract, using the **upsertfarm** and **upsertdao** actions:

	**Parameters**
	- **user** *the authorized wallet you are using to submit this transaction*
	- **parts** *the amount of payment shares to allocate to this farm or DAO*
	- **farm_name/dao_name** *the name of the farm or DAO you are allocating shares to*
	- **farm_contract** *specify if the NFT farm is on the old (waxdaofarmer) or the new (farms.waxdao) contract*

	This lets the contract know which farms/daos to split the royalty payments up between.

	*Note 1: the "upsert" actions will also work as "update" actions, if you ever want to change the amount of shares given to a farm or DAO*

	*Note 2: you can also remove farms or daos from the configuration by using the "removefarm" and/or "removedao" actions*

3. You can also add other WAX wallets and give them shares of any incoming transfers, using the **upsertwallet** action

	**Parameters**
	- **user** *the authorized wallet you are using to submit this transaction*
	- **parts** *the amount of payment shares to allocate to this wallet*
	- **wax_address** *the wax address you are allocating shares to*
	- **memo** *(optional) the custom memo you want to include when sending to this wallet*

	**Update:** *you can now keep some parts in the contract wallet if you want to. Simply enter the contract wallet's address as the wax_address. For example, if you uploaded this contract to the "waxdao" account, then you would put "waxdao" as the wax_address, and the parts for waxdao would remain in the waxdao wallet*

	*Note: removewallet is the action for removing a wallet from this list*

4. If you want the contract to accept/split other tokens **besides WAX**, you need to enable this setting using the **allowalltokn** action.
	
	**Parameters**
	- **user** *the authorized wallet you are using to submit this transaction*
	- **is_allowed** *put 0 to NOT allow other tokens; put 1 to allow other tokens*

5. You can now add rules for specific tokens, using the **addrule** action

	**Parameters**
	- **user** *the authorized wallet you are using to submit this transaction*
	- **token_symbol** *the symbol (including decimals and token name) of the token you want to add a rule for. For example, the symbol for WAX is: 8,WAX*
	- **contract** *the contract address of the token you want to add a rule for*

	*Note: removerule is the action for removing a rule from the contract*

6. If you added any rules for specific tokens, you can enter the payment receivers and parts for those rules, by using the **ruledaoupdt** (for DAOs), **rulefarmupdt** (for farms), and **rulewlltupdt** (for wallets). You can **remove** payment receivers by using the **ruledaormv, rulefarmrmv, and rulewlltrmv** actions.

7. If you haven't done so already, add the **eosio.code** permission to the account where the contract is stored.

	This can be done using the **[eosio.code tool on naw.io](https://wax-tools.naw.io/#show-tab=code-tab)**

	**Note:** this is a third party tool, but it is built by a trusted, long-standing WAX community member (Nick Kusters). You can also add this permission on your own, provided you know what you are doing.


## What's Next

If you used WaxDAO to deploy this contract (in other words, if you supported the contract author by buying/burning an NFT), then you can just sit back - WaxDAO has a script running that checks all whitelisted accounts multiple times per day. If the account has a royalty balance to claim, AND if it has been at least **hours_between_claims** since the **last_claim**, then it will automatically get claimed and transferred to the appropriate farms/daos without you needing to do anything.

However, if you did not burn an NFT to get whitelisted for this contract, and you chose to manually compile the open source code and deploy it yourself - then you are basically on your own as far as executing the **claimroyalty** action every x amount of hours. You can write your own script for that, or do it manually - that is your call. But if you want support from WaxDAO, we'd recommend the "burn an NFT" option (and we'd greatly appreciate the support!)

---

### *Want to support us for building this tool, and others like it? Feel free to add [waxdaovault2](https://waxblock.io/account/waxdaovault2) as a payment receiver when you deploy this contract ;)*
