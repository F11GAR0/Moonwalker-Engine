#include "walker.h"
#include "LogCommandParser.h"
#include "RakNetworkFactory.h"
#include "RakClientInterface.h"
#include "RakClient.h"
#include "RakPeerInterface.h"
#include "RakPeer.h"

RakClientInterface* RakNetworkFactory::GetRakClientInterface( void )
{
	return new RakClient;
}

void RakNetworkFactory::DestroyRakClientInterface( RakClientInterface* i )
{
	delete ( RakClient* ) i;
}
