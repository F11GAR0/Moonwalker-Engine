#ifndef __RAK_CLIENTINTERFACE_H
#define __RAK_CLIENTINTERFACE_H

#include "NetworkTypes.h"
#include "PacketPriority.h"
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "RakNetStatistics.h" 

class RakClientInterface
{
public:
	virtual ~RakClientInterface() {}
	virtual bool Connect( const char* host, unsigned short serverPort, unsigned short clientPort, unsigned int depreciated, int threadSleepTimer )=0;
	virtual void Disconnect( unsigned int blockDuration, unsigned char orderingChannel=0 )=0;
	virtual void InitializeSecurity( const char *privKeyP, const char *privKeyQ )=0;
	virtual void SetPassword( const char *_password )=0;
	virtual bool HasPassword( void ) const=0;
	virtual bool Send( const char *data, const int length, PacketPriority priority, PacketReliability reliability, char orderingChannel )=0;
	virtual bool Send( RakNet::BitStream * bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel )=0;
	virtual Packet* Receive( void )=0;
	virtual void DeallocatePacket( Packet *packet )=0;
	virtual void PingServer( void )=0;
	virtual void PingServer( const char* host, unsigned short serverPort, unsigned short clientPort, bool onlyReplyOnAcceptingConnections )=0;
	virtual int GetAveragePing( void )=0;
	virtual int GetLastPing( void ) const=0;
	virtual int GetLowestPing( void ) const=0;
	virtual int GetPlayerPing( const PlayerID playerId )=0;
	virtual void StartOccasionalPing( void )=0;
	virtual void StopOccasionalPing( void )=0;
	virtual bool IsConnected( void ) const=0;
	virtual unsigned int GetSynchronizedRandomInteger( void ) const=0;
	virtual bool GenerateCompressionLayer( unsigned int inputFrequencyTable[ 256 ], bool inputLayer )=0;
	virtual bool DeleteCompressionLayer( bool inputLayer )=0;
	virtual void RegisterAsRemoteProcedureCall( int* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) )=0;
	virtual void RegisterClassMemberRPC( int* uniqueID, void *functionPointer )=0;
	virtual void UnregisterAsRemoteProcedureCall( int* uniqueID )=0;
	virtual bool RPC( int* uniqueID, const char *data, unsigned int bitLength, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp)=0;
	virtual bool RPC( int* uniqueID, RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp)=0;
	virtual bool RPC_( int* uniqueID, RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp, NetworkID networkID)=0;
	virtual bool GetSendFrequencyTable( unsigned int outputFrequencyTable[ 256 ] )=0;
	virtual float GetCompressionRatio( void ) const=0;
	virtual float GetDecompressionRatio( void ) const=0;
	virtual void AttachPlugin( PluginInterface *messageHandler )=0;
	virtual void DetachPlugin( PluginInterface *messageHandler )=0;
	virtual RakNet::BitStream * GetStaticServerData( void )=0;
	virtual void SetStaticServerData( const char *data, const int length )=0;
	virtual RakNet::BitStream * GetStaticClientData( const PlayerID playerId )=0;
	virtual void SetStaticClientData( const PlayerID playerId, const char *data, const int length )=0;
	virtual void SendStaticClientDataToServer( void )=0;
	virtual PlayerID GetServerID( void ) const=0;
	virtual PlayerID GetPlayerID( void ) const=0;
	virtual PlayerID GetInternalID( void ) const=0;
	virtual const char* PlayerIDToDottedIP( const PlayerID playerId ) const=0;
	virtual void PushBackPacket( Packet *packet, bool pushAtHead )=0;
	virtual void SetRouterInterface( RouterInterface *routerInterface )=0;
	virtual void RemoveRouterInterface( RouterInterface *routerInterface )=0;
	virtual void SetTimeoutTime( RakNetTime timeMS )=0;
	virtual bool SetMTUSize( int size )=0;
	virtual int GetMTUSize( void ) const=0;	
	virtual void AllowConnectionResponseIPMigration( bool allow )=0;
	virtual void AdvertiseSystem( const char *host, unsigned short remotePort, const char *data, int dataLength )=0;
	virtual RakNetStatisticsStruct * const GetStatistics( void )=0;
	virtual void ApplyNetworkSimulator( double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance)=0;
	virtual bool IsNetworkSimulatorActive( void )=0;
	virtual PlayerIndex GetPlayerIndex( void )=0;
};
#endif
