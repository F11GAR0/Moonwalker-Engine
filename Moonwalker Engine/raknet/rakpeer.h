/// \file
/// \brief The main class used for data transmission and most of RakNet's functionality.
///
/// This file is part of RakNet Copyright 2003 Kevin Jenkins.
///
/// Usage of RakNet is subject to the appropriate license agreement.
/// Creative Commons Licensees are subject to the
/// license found at
/// http://creativecommons.org/licenses/by-nc/2.5/
/// Single application licensees are subject to the license found at
/// http://www.rakkarsoft.com/SingleApplicationLicense.html
/// Custom license users are subject to the terms therein.
/// GPL license users are subject to the GNU General Public
/// License as published by the Free
/// Software Foundation; either version 2 of the License, or (at your
/// option) any later version.

#ifndef __RAK_PEER_H
#define __RAK_PEER_H

#include "Export.h"
#include "RakPeerInterface.h"
#include "ReliabilityLayer.h"
#include "RPCNode.h"
#include "RSACrypt.h"
#include "BitStream.h"
#include "SingleProducerConsumer.h"
#include "RPCMap.h"
#include "SimpleMutex.h"
#include "DS_OrderedList.h"

class HuffmanEncodingTree;
class PluginInterface;

// Sucks but this struct has to be outside the class.  Inside and DevCPP won't let you refer to the struct as RakPeer::PlayerIDAndIndex while GCC
// forces you to do RakPeer::PlayerIDAndIndex
struct PlayerIDAndIndex{PlayerID playerId;unsigned index;};
int RAK_DLL_EXPORT PlayerIDAndIndexComp( const PlayerID &key, const PlayerIDAndIndex &data ); // GCC requires RakPeer::PlayerIDAndIndex or it won't compile

/// The primary interface for RakNet, RakPeer contains all major functions for the library.
/// See the individual functions for what the class can do.
/// \brief The main interface for network communications
class RAK_DLL_EXPORT RakPeer : public RakPeerInterface
{
public:
	///Constructor
	RakPeer();

	///Destructor
	virtual ~RakPeer();

	// --------------------------------------------------------------------------------------------Major Low Level Functions - Functions needed by most users--------------------------------------------------------------------------------------------
	/// \brief Starts the network threads, opens the listen port.
	/// You must call this before calling Connect().
	/// Multiple calls while already active are ignored.  To call this function again with different settings, you must first call Disconnect().
	/// \note Call SetMaximumIncomingConnections if you want to accept incoming connections
	/// \param[in] maxConnections The maximum number of connections between this instance of RakPeer and another instance of RakPeer. Required so the network can preallocate and for thread safety. A pure client would set this to 1.  A pure server would set it to the number of allowed clients.- A hybrid would set it to the sum of both types of connections
	/// \param[in] localPort The port to listen for connections on.
	/// \param[in] _threadSleepTimer How many ms to Sleep each internal update cycle (30 to give the game priority, 0 for regular (recommended), -1 to not Sleep() (may be slower))
	/// \param[in] forceHostAddress Can force RakNet to use a particular IP to host on.  Pass 0 to automatically pick an IP
	/// \return False on failure (can't create socket or thread), true on success.
	bool Initialize( unsigned short maxConnections, unsigned short localPort, int _threadSleepTimer, const char *forceHostAddress=0 );

	/// Secures connections though a combination of SHA1, AES128, SYN Cookies, and RSA to prevent connection spoofing, replay attacks, data eavesdropping, packet tampering, and MitM attacks.
	/// There is a significant amount of processing and a slight amount of bandwidth overhead for this feature.
	/// If you accept connections, you must call this or else secure connections will not be enabled for incoming connections.
	/// If you are connecting to another system, you can call this with values for the (e and p,q) public keys before connecting to prevent MitM
	/// \pre Must be called before Initialize
	/// \param[in] pubKeyE A pointer to the public keys from the RSACrypt class.  
	/// \param[in] pubKeyN A pointer to the public keys from the RSACrypt class. 
	/// \param[in] privKeyP Public key generated from the RSACrypt class.  
	/// \param[in] privKeyQ Public key generated from the RSACrypt class.  If the private keys are 0, then a new key will be generated when this function is called@see the Encryption sample
	void InitializeSecurity(const char *pubKeyE, const char *pubKeyN, const char *privKeyP, const char *privKeyQ );

	/// Disables all security.
	/// \note Must be called while offline
	void DisableSecurity( void );

	/// Sets how many incoming connections are allowed. If this is less than the number of players currently connected,
	/// no more players will be allowed to connect.  If this is greater than the maximum number of peers allowed,
	/// it will be reduced to the maximum number of peers allowed.  Defaults to 0.
	/// \param[in] numberAllowed Maximum number of incoming connections allowed.
	void SetMaximumIncomingConnections( unsigned short numberAllowed );

	/// Returns the value passed to SetMaximumIncomingConnections()
	/// \return the maximum number of incoming connections, which is always <= maxConnections
	unsigned short GetMaximumIncomingConnections( void ) const;

	/// Sets the password incoming connections must match in the call to Connect (defaults to none). Pass 0 to passwordData to specify no password
	/// This is a way to set a low level password for all incoming connections.  To selectively reject connections, implement your own scheme using CloseConnection() to remove unwanted connections
	/// \param[in] passwordData A data block that incoming connections must match.  This can be just a password, or can be a stream of data. Specify 0 for no password data
	/// \param[in] passwordDataLength The length in bytes of passwordData
	void SetIncomingPassword( const char* passwordData, int passwordDataLength );

	/// Gets the password passed to SetIncomingPassword
	/// \param[out] passwordData  Should point to a block large enough to hold the password data you passed to SetIncomingPassword()
	/// \param[in,out] passwordDataLength Maximum size of the array passwordData.  Modified to hold the number of bytes actually written
	void GetIncomingPassword( char* passwordData, int *passwordDataLength  );

	/// \brief Connect to the specified host (ip or domain name) and server port.
	/// Calling Connect and not calling SetMaximumIncomingConnections acts as a dedicated client.
	/// Calling both acts as a true peer. This is a non-blocking connection.
	/// You know the connection is successful when IsConnected() returns true or Receive() gets a message with the type identifier ID_CONNECTION_ACCEPTED.
	/// If the connection is not successful, such as a rejected connection or no response then neither of these things will happen.
	/// \pre Requires that you first call Initialize
	/// \param[in] host Either a dotted IP address or a domain name
	/// \param[in] remotePort Which port to connect to on the remote machine.
	/// \param[in] passwordData A data block that must match the data block on the server passed to SetIncomingPassword.  This can be a string or can be a stream of data.  Use 0 for no password.
	/// \param[in] passwordDataLength The length in bytes of passwordData
	/// \return True on successful initiation. False on incorrect parameters, internal error, or too many existing peers.  Returning true does not mean you connected!
	bool Connect( const char* host, unsigned short remotePort, char* passwordData, int passwordDataLength );

	/// \brief Stops the network threads and closes all connections.
	/// \param[in] blockDuration How long you should wait for all remaining messages to go out, including ID_DISCONNECTION_NOTIFICATION.  If 0, it doesn't wait at all.
	/// \param[in] orderingChannel If blockDuration > 0, ID_DISCONNECTION_NOTIFICATION will be sent on this channel
	/// If you set it to 0 then the disconnection notification won't be sent
	void Disconnect( unsigned int blockDuration, unsigned char orderingChannel=0 );

	/// Returns if the network thread is running
	/// \return true if the network thread is running, false otherwise
	bool IsActive( void ) const;

	/// Fills the array remoteSystems with the SystemID of all the systems we are connected to
	/// \param[out] remoteSystems An array of PlayerID structures to be filled with the PlayerIDs of the systems we are connected to. Pass 0 to remoteSystems to only get the number of systems we are connected to
	/// \param[in, out] numberOfSystems As input, the size of remoteSystems array.  As output, the number of elements put into the array 
	bool GetConnectionList( PlayerID *remoteSystems, unsigned short *numberOfSystems ) const;

	/// Sends a block of data to the specified system that you are connected to.
	/// This function only works while the connected
	/// \param[in] data The block of data to send
	/// \param[in] length The size in bytes of the data to send
	/// \param[in] priority What priority level to send on.  See PacketPriority.h
	/// \param[in] reliability How reliability to send this data.  See PacketPriority.h
	/// \param[in] orderingChannel When using ordered or sequenced messages, what channel to order these on. Messages are only ordered relative to other messages on the same stream
	/// \param[in] playerId Who to send this packet to, or in the case of broadcasting who not to send it to.  Use UNASSIGNED_PLAYER_ID to specify none
	/// \param[in] broadcast True to send this packet to all connected systems. If true, then playerId specifies who not to send the packet to.
	/// \return False if we are not connected to the specified recipient.  True otherwise
	bool Send( const char *data, const int length, PacketPriority priority, PacketReliability reliability, char orderingChannel, PlayerID playerId, bool broadcast );

	/// Sends a block of data to the specified system that you are connected to.  Same as the above version, but takes a BitStream as input.
	/// \param[in] bitStream The bitstream to send
	/// \param[in] priority What priority level to send on.  See PacketPriority.h
	/// \param[in] reliability How reliability to send this data.  See PacketPriority.h
	/// \param[in] orderingChannel When using ordered or sequenced messages, what channel to order these on. Messages are only ordered relative to other messages on the same stream
	/// \param[in] playerId Who to send this packet to, or in the case of broadcasting who not to send it to.  Use UNASSIGNED_PLAYER_ID to specify none
	/// \param[in] broadcast True to send this packet to all connected systems. If true, then playerId specifies who not to send the packet to.
	/// \return False if we are not connected to the specified recipient.  True otherwise
	bool Send( RakNet::BitStream * bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, PlayerID playerId, bool broadcast );

	/// Gets a message from the incoming message queue.
	/// Use DeallocatePacket() to deallocate the message after you are done with it.
	/// User-thread functions, such as RPC calls and the plugin function PluginInterface::Update occur here.
	/// \return 0 if no packets are waiting to be handled, otherwise a pointer to a packet.
	/// sa NetworkTypes.h contains struct Packet
	Packet* Receive( void );

	/// Call this to deallocate a message returned by Receive() when you are done handling it.
	/// \param[in] packet The message to deallocate.	
	void DeallocatePacket( Packet *packet );

	/// Return the total number of connections we are allowed
	unsigned short GetMaximumNumberOfPeers( void ) const;

	// --------------------------------------------------------------------------------------------Remote Procedure Call Functions - Functions to initialize and perform RPC--------------------------------------------------------------------------------------------
	/// \ingroup RAKNET_RPC
	/// Register a C or static member function as available for calling as a remote procedure call
	/// \param[in] uniqueID A null-terminated unique string to identify this procedure.  See RegisterClassMemberRPC() for class member functions.
	/// \param[in] functionPointer(...) The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered
	void RegisterAsRemoteProcedureCall( int* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) );

	/// \ingroup RAKNET_RPC
	/// Register a C++ member function as available for calling as a remote procedure call.
	/// \param[in] uniqueID A null terminated string to identify this procedure. Recommended you use the macro REGISTER_CLASS_MEMBER_RPC to create the string.  Use RegisterAsRemoteProcedureCall() for static functions.
	/// \param[in] functionPointer The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered with UnregisterAsRemoteProcedureCall
	/// \sa The sample ObjectMemberRPC.cpp
	void RegisterClassMemberRPC( int* uniqueID, void *functionPointer );

	/// \ingroup RAKNET_RPC
	/// Unregisters a C function as available for calling as a remote procedure call that was formerly registered with RegisterAsRemoteProcedureCall. Only call offline.
	/// \param[in] uniqueID A string of only letters to identify this procedure.  Recommended you use the macro CLASS_MEMBER_ID for class member functions.
	void UnregisterAsRemoteProcedureCall( int* uniqueID );

	/// \ingroup RAKNET_RPC
	/// Calls a C function on the remote system that was already registered using RegisterAsRemoteProcedureCall().
	/// \param[in] uniqueID A NULL terminated string identifying the function to call.  Recommended you use the macro CLASS_MEMBER_ID for class member functions.
	/// \param[in] data The data to send
	/// \param[in] bitLength The number of bits of \a data
	/// \param[in] priority What priority level to send on. See PacketPriority.h.
	/// \param[in] reliability How reliability to send this data. See PacketPriority.h.
	/// \param[in] orderingChannel When using ordered or sequenced message, what channel to order these on.
	/// \param[in] playerId Who to send this message to, or in the case of broadcasting who not to send it to.  Use UNASSIGNED_PLAYER_ID to specify none
	/// \param[in] broadcast True to send this packet to all connected systems. If true, then playerId specifies who not to send the packet to.
	/// \param[in] shiftTimestamp True to add a timestamp to your data, such that the first byte is ID_TIMESTAMP and the next sizeof(RakNetTime) is the timestamp.
	/// \param[in] networkID For static functions, pass UNASSIGNED_NETWORK_ID.  For member functions, you must derive from NetworkIDGenerator and pass the value returned by NetworkIDGenerator::GetNetworkID for that object.
	/// \param[in] replyFromTarget If 0, this function is non-blocking.  Otherwise it will block while waiting for a reply from the target procedure, which should be remotely written to RPCParameters::replyToSender and copied to replyFromTarget.  The block will return early on disconnect or if the sent packet is unreliable and more than 3X the ping has elapsed.
	/// \return True on a successful packet send (this does not indicate the recipient performed the call), false on failure
	bool RPC( int* uniqueID, const char *data, unsigned int bitLength, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp);
	bool RPC( int* uniqueID, RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp);
	bool RPC_( int* uniqueID, RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp, NetworkID networkID);
	void CloseConnection( const PlayerID target, bool sendDisconnectionNotification, unsigned char orderingChannel=0 );
	int GetIndexFromPlayerID( const PlayerID playerId );
	PlayerID GetPlayerIDFromIndex( int index );
	void AddToBanList( const char *IP, RakNetTime milliseconds=0 );
	void RemoveFromBanList( const char *IP );
	void ClearBanList( void );
	bool IsBanned( const char *IP );
	void Ping( const PlayerID target );
	void Ping( const char* host, unsigned short remotePort, bool onlyReplyOnAcceptingConnections );
	int GetAveragePing( const PlayerID playerId );
	int GetLastPing( const PlayerID playerId ) const;
	int GetLowestPing( const PlayerID playerId ) const;
	void SetOccasionalPing( bool doPing );
	RakNet::BitStream * GetRemoteStaticData( const PlayerID playerId );

	/// All systems have a block of data associated with them, for user use.  This block of data can be used to easily
	/// specify typical system data that you want to know on connection, such as the player's name.
	/// \param[in] playerId Whose static data to change.  Use your own playerId to change your own static data
	/// \param[in] data a block of data to store
	/// \param[in] length The length of data in bytes 
	void SetRemoteStaticData( const PlayerID playerId, const char *data, const int length );

	/// Sends your static data to the specified system. This is automatically done on connection.
	/// You should call this when you change your static data.To send the static data of another system (such as relaying their data) you should do this
	/// normally with Send)
	/// \param[in] target Who to send your static data to.  Specify UNASSIGNED_PLAYER_ID to broadcast to all
	void SendStaticData( const PlayerID target );

	/// Sets the data to send along with a LAN server discovery or offline ping reply.
	/// \a length should be under 400 bytes, as a security measure against flood attacks
	/// \param[in] data a block of data to store, or 0 for none
	/// \param[in] length The length of data in bytes, or 0 for none
	/// \sa Ping.cpp
	void SetOfflinePingResponse( const char *data, const unsigned int length );
	
	//--------------------------------------------------------------------------------------------Network Functions - Functions dealing with the network in general--------------------------------------------------------------------------------------------
	/// Return the unique address identifier that represents you on the the network and is based on your local IP / port.
	/// \return the identifier of your system internally, which may not be how other systems see if you if you are behind a NAT or proxy
	PlayerID GetInternalID( void ) const;

	/// Return the unique address identifier that represents you on the the network and is based on your externalIP / port
	/// (the IP / port the specified player uses to communicate with you)
	/// \param[in] target Which remote system you are referring to for your external ID.  Usually the same for all systems, unless you have two or more network cards.
	PlayerID GetExternalID( const PlayerID target ) const;

	/// Set the time, in MS, to use before considering ourselves disconnected after not being able to deliver a reliable message.
	/// Default time is 10,000 or 10 seconds in release and 30,000 or 30 seconds in debug.
    /// \param[in] timeMS Time, in MS
	/// \param[in] target Which system to do this for
	void SetTimeoutTime( RakNetTime timeMS, const PlayerID target );

	/// Set the MTU per datagram.  It's important to set this correctly - otherwise packets will be needlessly split, decreasing performance and throughput.
	/// Maximum allowed size is MAXIMUM_MTU_SIZE.
	/// Too high of a value will cause packets not to arrive at worst and be fragmented at best.
	/// Too low of a value will split packets unnecessarily.
	/// Recommended size is 1500
	/// sa MTUSize.h
	/// \pre Can only be called when not connected.
	/// \return false on failure (we are connected), else true
	bool SetMTUSize( int size );

	/// Returns the current MTU size
	/// \return The current MTU size
	int GetMTUSize( void ) const;

	/// Returns the number of IP addresses this system has internally. Get the actual addresses from GetLocalIP()
	unsigned GetNumberOfAddresses( void );

	/// Returns an IP address at index 0 to GetNumberOfAddresses-1
	const char* GetLocalIP( unsigned int index );

	/// TODO - depreciate this
	/// Returns the dotted IP address for the specified playerId
	/// \param[in] playerId Any player ID other than UNASSIGNED_PLAYER_ID, even if that player is not currently connected
	const char* PlayerIDToDottedIP( const PlayerID playerId ) const;

	// TODO - depreciate this
	/// Converts a dotted IP to a playerId
	/// \param[in]  host Either a dotted IP address or a domain name
	/// \param[in]  remotePort Which port to connect to on the remote machine.
	/// \param[out] playerId  The result of this operation
	void IPToPlayerID( const char* host, unsigned short remotePort, PlayerID *playerId );

	/// Allow or disallow connection responses from any IP. Normally this should be false, but may be necessary
	/// when connecting to servers with multiple IP addresses.
	/// \param[in] allow - True to allow this behavior, false to not allow. Defaults to false. Value persists between connections
	void AllowConnectionResponseIPMigration( bool allow );

	/// Sends a one byte message ID_ADVERTISE_SYSTEM to the remote unconnected system.
	/// This will tell the remote system our external IP outside the LAN along with some user data.
	/// \pre The sender and recipient must already be started via a successful call to Initialize
	/// \param[in] host Either a dotted IP address or a domain name
	/// \param[in] remotePort Which port to connect to on the remote machine.
	/// \param[in] data Optional data to append to the packet.
	/// \param[in] dataLength length of data in bytes.  Use 0 if no data.
	void AdvertiseSystem( const char *host, unsigned short remotePort, const char *data, int dataLength );

	/// Controls how often to return ID_DOWNLOAD_PROGRESS for large message downloads.
	/// ID_DOWNLOAD_PROGRESS is returned to indicate a new partial message chunk, roughly the MTU size, has arrived
	/// As it can be slow or cumbersome to get this notification for every chunk, you can set the interval at which it is returned.
	/// Defaults to 0 (never return this notification)
	/// \param[in] interval How many messages to use as an interval
	void SetSplitMessageProgressInterval(int interval);

	/// Set how long to wait before giving up on sending an unreliable message
	/// Useful if the network is clogged up.
	/// Set to 0 or less to never timeout.  Defaults to 0.
	/// \param[in] timeoutMS How many ms to wait before simply not sending an unreliable message.
	void SetUnreliableTimeout(RakNetTime timeoutMS);

	// --------------------------------------------------------------------------------------------Compression Functions - Functions related to the compression layer--------------------------------------------------------------------------------------------
	/// Enables or disables frequency table tracking.  This is required to get a frequency table, which is used in GenerateCompressionLayer()
	/// This value persists between connect calls and defaults to false (no frequency tracking)
	/// \pre You can call this at any time - however you SHOULD only call it when disconnected.  Otherwise you will only trackpart of the values sent over the network.
	/// \param[in] doCompile True to enable tracking 
	void SetCompileFrequencyTable( bool doCompile );

	/// Returns the frequency of outgoing bytes into output frequency table
	/// The purpose is to save to file as either a master frequency table from a sample game session for passing to
	/// GenerateCompressionLayer() 
	/// \pre You should only call this when disconnected. Requires that you first enable data frequency tracking by calling SetCompileFrequencyTable(true)
	/// \param[out] outputFrequencyTable  The frequency of each corresponding byte
	/// \return False (failure) if connected or if frequency table tracking is not enabled. Otherwise true (success)
	bool GetOutgoingFrequencyTable( unsigned int outputFrequencyTable[ 256 ] );
	bool GenerateCompressionLayer( unsigned int inputFrequencyTable[ 256 ], bool inputLayer );
	bool DeleteCompressionLayer( bool inputLayer );
	float GetCompressionRatio( void ) const;
	float GetDecompressionRatio( void ) const;
	void AttachPlugin( PluginInterface *plugin );
	void DetachPlugin( PluginInterface *messageHandler );
	void PushBackPacket( Packet *packet, bool pushAtHead );
	void SetRouterInterface( RouterInterface *routerInterface );
	void RemoveRouterInterface( RouterInterface *routerInterface );
    void ApplyNetworkSimulator( double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance);
	bool IsNetworkSimulatorActive( void );
	RakNetStatisticsStruct * const GetStatistics( const PlayerID playerId );
	RPCMap *GetRPCMap( const PlayerID playerId);
	struct PingAndClockDifferential
	{
		unsigned short pingTime;
		RakNetTime clockDifferential;
	};
	struct RemoteSystemStruct
	{
		bool isActive; // Is this structure in use?
		PlayerID playerId;  /// The remote system associated with this reliability layer
		PlayerID myExternalPlayerId;  /// Your own IP, as reported by the remote system
		ReliabilityLayer reliabilityLayer;  /// The reliability layer associated with this player
		bool weInitiatedTheConnection; /// True if we started this connection via Connect.  False if someone else connected to us.
		PingAndClockDifferential pingAndClockDifferential[ PING_TIMES_ARRAY_SIZE ];  /// last x ping times and calculated clock differentials with it
		int pingAndClockDifferentialWriteIndex;  /// The index we are writing into the pingAndClockDifferential circular buffer
		unsigned short lowestPing; ///The lowest ping value encountered
		RakNetTime nextPingTime;  /// When to next ping this player
		RakNetTime lastReliableSend; /// When did the last reliable send occur.  Reliable sends must occur at least once every timeoutTime/2 units to notice disconnects
		RakNet::BitStream staticData; /// static data.  This cannot be a pointer because it might be accessed in another thread.
		RakNetTime connectionTime; /// connection time, if active.
		unsigned char AESKey[ 16 ]; /// Security key.
		bool setAESKey; /// true if security is enabled.
		RPCMap rpcMap; /// Mapping of RPC calls to single byte integers to save transmission bandwidth.
		enum ConnectMode {NO_ACTION, DISCONNECT_ASAP, DISCONNECT_ASAP_SILENTLY, DISCONNECT_ON_NO_ACK, REQUESTED_CONNECTION, HANDLING_CONNECTION_REQUEST, UNVERIFIED_SENDER, SET_ENCRYPTION_ON_MULTIPLE_16_BYTE_PACKET, CONNECTED} connectMode;
	};

protected:
#ifdef _WIN32
	friend void __stdcall ProcessPortUnreachable( const unsigned int binaryAddress, const unsigned short port, RakPeer *rakPeer );
	friend void __stdcall ProcessNetworkPacket( const unsigned int binaryAddress, const unsigned short port, const char *data, const int length, RakPeer *rakPeer );
	friend unsigned __stdcall UpdateNetworkLoop( LPVOID arguments );
#else
	friend void ProcessPortUnreachable( const unsigned int binaryAddress, const unsigned short port, RakPeer *rakPeer );
	friend void ProcessNetworkPacket( const unsigned int binaryAddress, const unsigned short port, const char *data, const int length, RakPeer *rakPeer );
	friend void* UpdateNetworkLoop( void* arguments );
#endif
	Packet* ReceiveIgnoreRPC( void );
	int GetIndexFromPlayerID( const PlayerID playerId, bool calledFromNetworkThread );
	bool SendConnectionRequest( const char* host, unsigned short remotePort, char* passwordData, int passwordDataLength );
	RemoteSystemStruct *GetRemoteSystemFromPlayerID( const PlayerID playerID, bool calledFromNetworkThread, bool onlyActive) const;
	void ParseConnectionRequestPacket( RakPeer::RemoteSystemStruct *remoteSystem, PlayerID playerId, const char *data, int byteSize);
	void OnConnectionRequest( RakPeer::RemoteSystemStruct *remoteSystem, unsigned char *AESKey, bool setAESKey );
	void NotifyAndFlagForDisconnect( const PlayerID playerId, bool performImmediate, unsigned char orderingChannel );
	unsigned short GetNumberOfRemoteInitiatedConnections( void ) const;
	RemoteSystemStruct * AssignPlayerIDToRemoteSystemList( const PlayerID playerId, RemoteSystemStruct::ConnectMode connectionMode );
	void ShiftIncomingTimestamp( unsigned char *data, PlayerID playerId ) const;
	RakNetTime GetBestClockDifferential( const PlayerID playerId ) const;
	bool HandleRPCPacket( const char *data, int length, PlayerID playerId );
	void HandleRPCReplyPacket( const char *data, int length, PlayerID playerId );
	volatile bool endThreads;
	volatile bool isMainLoopThreadActive;
	bool occasionalPing;
	unsigned short maximumNumberOfPeers;
	unsigned short maximumIncomingConnections;
	RakNet::BitStream localStaticData, offlinePingResponse;
	PlayerID myPlayerId;
	char incomingPassword[256];
	unsigned char incomingPasswordLength;
	RemoteSystemStruct* remoteSystemList;
	DataStructures::OrderedList<PlayerID, PlayerIDAndIndex, PlayerIDAndIndexComp> remoteSystemLookup;
	
	enum
	{
		offlinePingResponse_Mutex,
		NUMBER_OF_RAKPEER_MUTEXES
	};
	SimpleMutex rakPeerMutexes[ NUMBER_OF_RAKPEER_MUTEXES ];
	///RunUpdateCycle is not thread safe but we don't need to mutex calls. Just skip calls if it is running already

	bool updateCycleIsRunning;
	///The list of people we have tried to connect to recently

	//DataStructures::Queue<RequestedConnectionStruct*> requestedConnectionsList;
	///Data that both the client and the server needs

	unsigned int bytesSentPerSecond, bytesReceivedPerSecond;
	// bool isSocketLayerBlocking;
	// bool continualPing,isRecvfromThreadActive,isMainLoopThreadActive, endThreads, isSocketLayerBlocking;
	unsigned int validationInteger;
#ifdef _WIN32
	HANDLE
#else
	pthread_t
#endif
		processPacketsThreadHandle, recvfromThreadHandle;
	SimpleMutex incomingQueueMutex, banListMutex; //,synchronizedMemoryQueueMutex, automaticVariableSynchronizationMutex;
	//DataStructures::Queue<Packet *> incomingpacketSingleProducerConsumer; //, synchronizedMemorypacketSingleProducerConsumer;
	// BitStream enumerationData;

	/// @brief Automatic Variable Synchronization Mechanism
	/// automatic variable synchronization takes a primary and secondary identifier
	/// The unique primary identifier is the index into the automaticVariableSynchronizationList
	/// The unique secondary identifier (UNASSIGNED_NETWORK_ID for none) is in an unsorted list of memory blocks
	struct MemoryBlock
	{
		char *original, *copy;
		unsigned short size;
		NetworkID secondaryID;
		bool isAuthority;
		bool ( *synchronizationRules ) ( char*, char* );
	};

	struct BanStruct
	{
		char *IP;
		RakNetTime timeout; // 0 for none
	};

	struct RequestedConnectionStruct
	{
		PlayerID playerId;
		RakNetTime nextRequestTime;
		unsigned char requestsMade;
		char *data;
		unsigned short dataLength;
		char outgoingPassword[256];
		unsigned char outgoingPasswordLength;
		enum {CONNECT=1, /*PING=2, PING_OPEN_CONNECTIONS=4,*/ /*ADVERTISE_SYSTEM=2*/} actionToTake;
	};

	//DataStructures::List<DataStructures::List<MemoryBlock>* > automaticVariableSynchronizationList;
	DataStructures::List<BanStruct*> banList;
	DataStructures::List<PluginInterface*> messageHandlerList;
	DataStructures::SingleProducerConsumer<RequestedConnectionStruct> requestedConnectionList;

	/// Compression stuff
	unsigned int frequencyTable[ 256 ];
	HuffmanEncodingTree *inputTree, *outputTree;
	unsigned int rawBytesSent, rawBytesReceived, compressedBytesSent, compressedBytesReceived;
	// void DecompressInput(RakNet::BitStream *bitStream);
	// void UpdateOutgoingFrequencyTable(RakNet::BitStream * bitStream);
	void GenerateSYNCookieRandomNumber( void );
	void SecuredConnectionResponse( const PlayerID playerId );
	void SecuredConnectionConfirmation( RakPeer::RemoteSystemStruct * remoteSystem, char* data );
	bool RunUpdateCycle( void );
	// void RunMutexedUpdateCycle(void);

	struct BufferedCommandStruct
	{
		int numberOfBitsToSend;
		PacketPriority priority;
		PacketReliability reliability;
		char orderingChannel;
		PlayerID playerId;
		bool broadcast;
		RemoteSystemStruct::ConnectMode connectionMode;
		NetworkID networkID;
		bool blockingCommand; // Only used for RPC
		char *data;
		enum {BCS_SEND, BCS_CLOSE_CONNECTION, /*BCS_RPC, BCS_RPC_SHIFT,*/ BCS_DO_NOTHING} command;
	};

	// Single producer single consumer queue using a linked list
	//BufferedCommandStruct* bufferedCommandReadIndex, bufferedCommandWriteIndex;
	DataStructures::SingleProducerConsumer<BufferedCommandStruct> bufferedCommands;

	bool AllowIncomingConnections(void) const;

	// Sends static data using immediate send mode or not (if called from user thread, put false for performImmediate.  If called from update thread, put true).
	// This is done for efficiency, so we don't buffer send calls that are from the network thread anyway
	void SendStaticDataInternal( const PlayerID target, bool performImmediate );
	void PingInternal( const PlayerID target, bool performImmediate );
	bool ValidSendTarget(PlayerID playerId, bool broadcast);
	// This stores the user send calls to be handled by the update thread.  This way we don't have thread contention over playerIDs
	void CloseConnectionInternal( const PlayerID target, bool sendDisconnectionNotification, bool performImmediate, unsigned char orderingChannel );
	void SendBuffered( const char *data, int numberOfBitsToSend, PacketPriority priority, PacketReliability reliability, char orderingChannel, PlayerID playerId, bool broadcast, RemoteSystemStruct::ConnectMode connectionMode );
	bool SendImmediate( char *data, int numberOfBitsToSend, PacketPriority priority, PacketReliability reliability, char orderingChannel, PlayerID playerId, bool broadcast, bool useCallerDataAllocation, RakNetTimeNS currentTime );
	//bool HandleBufferedRPC(BufferedCommandStruct *bcs, RakNetTime time);
	void ClearBufferedCommands(void);
	void ClearRequestedConnectionList(void);
	void AddPacketToProducer(Packet *p);
	//DataStructures::AVLBalancedBinarySearchTree<RPCNode> rpcTree;
	RPCMap rpcMap; // Can't use StrPtrHash because runtime insertions will screw up the indices
	int MTUSize;
	bool trackFrequencyTable;
	int threadSleepTimer;

	SOCKET connectionSocket;
#if defined (_WIN32) && defined(USE_WAIT_FOR_MULTIPLE_EVENTS)
	WSAEVENT recvEvent;
#endif

	// Used for RPC replies
	RakNet::BitStream *replyFromTargetBS;
	PlayerID replyFromTargetPlayer;
	bool replyFromTargetBroadcast;

	// Problem:
	// Waiting in function A:
	// Wait function gets RPC B:
	// 
	bool blockOnRPCReply;

	// For redirecting sends through the router plugin.  Unfortunate I have to use this architecture.
	RouterInterface *router;

	// Nobody would use the internet simulator in a final build.
#ifndef _RELEASE
	double _maxSendBPS;
	unsigned short _minExtraPing, _extraPingVariance;
#endif

#if !defined(_COMPATIBILITY_1)
	/// Encryption and security
	big::RSACrypt<RSA_BIT_SIZE> rsacrypt;
	big::u32 publicKeyE;
	RSA_BIT_SIZE publicKeyN;
	bool keysLocallyGenerated, usingSecurity;
	RakNetTime randomNumberExpirationTime;
	unsigned char newRandomNumber[ 20 ], oldRandomNumber[ 20 ];
#endif
    
	///How long it has been since things were updated by a call to receiveUpdate thread uses this to determine how long to sleep for
	//unsigned int lastUserUpdateCycle;
	/// True to allow connection accepted packets from anyone.  False to only allow these packets from servers we requested a connection to.
	bool allowConnectionResponseIPMigration;

	int splitMessageProgressInterval;
	RakNetTime unreliableTimeout;

	// The packetSingleProducerConsumer transfers the packets from the network thread to the user thread. The pushedBackPacket holds packets that couldn't be processed
	// immediately while waiting on blocked RPCs
	DataStructures::SingleProducerConsumer<Packet*> packetSingleProducerConsumer;
	//DataStructures::Queue<Packet*> pushedBackPacket, outOfOrderDeallocatedPacket;
	DataStructures::Queue<Packet*> packetPool;
};

#endif
