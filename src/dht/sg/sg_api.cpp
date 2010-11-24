/**
 * This is the p2p messaging component of the Seeks project,
 * a collaborative websearch overlay network.
 *
 * Copyright (C) 2010  Emmanuel Benazera, juban@free.fr
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sg_api.h"
#include "qprocess.h"
#include "l2_protob_rpc_client.h"

using lsh::qprocess;

namespace dht
{
   dht_err sg_api::find_sg(const SGNode &sgnode,
			   const DHTKey &sg_key, Location &node)
     {
	DHTKey host_key;
	NetAddress host_na;
	dht_err status = dht_api::findSuccessor(sgnode,sg_key,host_key,host_na);
	node = Location(host_key,host_na);
	return status;
     }
   
   dht_err sg_api::get_sg_peers(const SGNode &sgnode,
				const DHTKey &sg_key, 
				const Location &node,
				std::vector<Subscriber*> &peers)
     {
	// level 2 subscribe call with no subscription.
	DHTKey senderKey;
	NetAddress senderAddress;
	int status = DHT_ERR_OK;
	l2_protob_rpc_client *l2_client = dynamic_cast<l2_protob_rpc_client*>(sgnode._l1_client);
	l2_client->RPC_subscribe(node.getDHTKey(),
				 node.getNetAddress(),
				 senderKey,senderAddress, // empty
				 sg_key,peers,status);
	return status;
     }
  
   dht_err sg_api::get_sg_peers_and_subscribe(const SGNode &sgnode,
					      const DHTKey &sg_key,
					      const Location &node,
					      std::vector<Subscriber*> &peers)
     {
	// level 2 subscribe call with subscription.
	DHTVirtualNode *vnode = sgnode.find_closest_vnode(node.getDHTKey()); // grab the closest virtual node.
	DHTKey senderKey = vnode->getIdKey();
	NetAddress senderAddress = sgnode._l1_na;
	int status = DHT_ERR_OK;
	l2_protob_rpc_client *l2_client = static_cast<l2_protob_rpc_client*>(sgnode._l1_client);
	l2_client->RPC_subscribe(node.getDHTKey(),
				 node.getNetAddress(),
				 senderKey,senderAddress,
				 sg_key,peers,status);
	return status;
     }
   
} /* end of namespace. */