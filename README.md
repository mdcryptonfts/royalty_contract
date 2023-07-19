# Royalty Contract Documentation

## Before you bother with this contract, there are a few things you should be aware of.

- If you are trying to claim royalties from a collection where you do **not** own the private keys to the author wallet, then this contract is 100% useless for you. You **must** be able to upload this contract to the account who created the NFT collection (you can **not** currently do this with MyCloudWallet, at least not at the time of this writing. This may change in the future.)

- You will need ~925,000 bytes of RAM to store this contract on your account (about 375 WAX worth, at the time of this writing. Check [waxblock](https://waxblock.io/wallet/ram) for up to date RAM prices.)

- You will need to add the **eosio.code** permission to your account, in order for this contract to work. I will include instructions below.

- If you are planning on sending royalties to a WaxDAO NFT farm, the farm's reward token must be **WAX**. You will not be able to link your WaxDAO NFT farm to this contract unless: the farm exists, and the reward token is **WAX** (precision is **8**, contract is **eosio.token**).

- The atomicmarket contract stores **all** of your claimable WAX in **1 single balance**. This is very important. If you have multiple schemas under 1 collection, all of the royalties will go into this 1 balance. If you make someone a buy offer from the same wallet that owns the collection; and then that buy offer gets rejected, that WAX will also go into this same balance. This contract will not differentiate between what "came from secondary" and what is "your own WAX". Keep this in mind when setting up your collection etc.

- There is a setting in this contract's configuration that allows you to choose whether or not to process transfers for **other tokens that are not WAX**.

- You can also send funds to DAO treasuries, if you want to send them to a v2 DAO on WaxDAO.

- You can also send funds to **any wax address, and you can even provide a custom memo**. One example use case for this: if you create a drop on any NFT market, you can set this contract as the receiver. This contract will then split up funds between any farms, DAOs, or even regular WAX wallets that you choose. A lot of you have asked "Can I send funds from my Nefty or Hive drop, directly to my NFT farm?"... Well, with this contract, yes.. you can. 

## Instructions


1. The very first thing you need to do when you upload the contract to your account, is use the **initconfig** action (go to your account on waxblock, click the "Contract Actions" tab, then click **initconfig**). 

	This action initializes the **config** table with the default settings. If you want to adjust these settings, you can use the actions on this contract, which I will outline below.

	*optional: add authorized_users to the table using the master account (self), via the addauthacct action.*

2. Next step is to add any farms and/or v2 DAOs to the contract, using the **insertfarm** and **insertdao** actions:

	**Parameters**
	- **user** *the authorized wallet you are using to submit this transaction*
	- **parts** *the amount of payment shares to allocate to this farm or DAO*
	- **farm_name/dao_name** *the name of the farm or DAO you are allocating shares to*

	This lets the contract know which farms/daos to split the royalty payments up between.

	*Note 1: the "insert" actions will also work as "update" actions, if you ever want to change the amount of shares given to a farm or DAO*

	*Note 2: you can also remove farms or daos from the configuration by using the "removefarm" and/or "removedao" actions*

3. You can also add other WAX wallets and give them shares of any incoming transfers, using the **insertwallet** action

	**Parameters**
	- **user** *the authorized wallet you are using to submit this transaction*
	- **parts** *the amount of payment shares to allocate to this wallet*
	- **wax_address** *the wax address you are allocating shares to*
	- **memo** *(optional) the custom memo you want to include when sending to this wallet*

	*Note: removewallet is the action for removing a wallet from this list*

4. If you want the contract to accept/split other tokens **besides WAX**, you need to enable this setting using the **allowalltokn** action.
	
	**Parameters**
	- **user** *the authorized wallet you are using to submit this transaction*
	- **is_allowed** *put 0 to NOT allow other tokens; put 1 to allow other tokens*

5. If you haven't done so already, add the **eosio.code** permission to the account where the contract is stored.

	This can be done using the **[eosio.code tool on naw.io](https://wax-tools.naw.io/#show-tab=code-tab)**

	**Note:** this is a third party tool, but it is built by a trusted, long-standing WAX community member (Nick Kusters). You can also add this permission on your own, provided you know what you are doing.


## What's Next

If you used WaxDAO to deploy this contract (in other words, if you supported the contract author by buying/burning an NFT), then you can just sit back - WaxDAO has a script running that checks all whitelisted accounts multiple times per day. If the account has a royalty balance to claim, AND if it has been at least **hours_between_claims** since the **last_claim**, then it will automatically get claimed and transferred to the appropriate farms/daos without you needing to do anything.

However, if you did not burn an NFT to get whitelisted for this contract, and you chose to manually compile the open source code and deploy it yourself - then you are basically on your own as far as executing the **claimroyalty** action every x amount of hours. You can write your own script for that, or do it manually - that is your call. But if you want support from WaxDAO, we'd recommend the "burn an NFT" option (and we'd greatly appreciate the support!)
