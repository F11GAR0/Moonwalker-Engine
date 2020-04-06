#pragma once
#include "walker.h"
#include "SAMPFUNCS\game_api\game_api.h"
#include <d3d9.h>
#include <d3dx9.h>

namespace STRUCTSV037R2 {

#pragma region DEFINES
#define SAMP_CHAT_INFO_OFFSET						0x21A0E4
#define SAMP_CHAT_INPUT_INFO_OFFSET					0x21A0E8
#define SAMP_KILL_INFO_OFFSET						0x21A0EC
#define SAMP_INFO_OFFSET							0x21A0F8
#define SAMP_MISC_INFO								0x21A10C
#define SAMP_SCOREBOARD_INFO						0x21A0B4
#define SAMP_COLOR_OFFSET							0x216378
#define SAMP_DRAWING_DISTANCE						0xD3DCC

#define SAMP_DRAWTEXTLABELS							0x71471
#define SAMP_ONFOOTSENDRATE							0xEC0A8
#define SAMP_INCARSENDRATE							0xEC0AC
#define SAMP_AIMSENDRATE							0xEC0B0

#define SAMP_FUNC_ADDTOCHATWND						0x64010
#define SAMP_FUNC_SPAWN								0x3AD0
#define SAMP_FUNC_REQUEST_SPAWN						0x3EC0
#define SAMP_FUNC_REQUEST_CLASS						0x56A0
#define SAMP_FUNC_DEATH								0x55E0
#define SAMP_FUNC_SETSPECIALACTION					0x30C0
#define SAMP_FUNC_SAY								0x57F0
#define SAMP_FUNC_SENDCMD							0x65C60
#define SAMP_FUNC_ADDCLIENTCMD						0x65AD0
#define SAMP_FUNC_NAMECHANGE						0xB290
#define SAMP_FUNC_RESTARTGAME						0xA060
#define SAMP_FUNC_ENCRYPT_PORT						0x1BC20
#define SAMP_FUNC_TOGGLECURSOR						0x9BD30
#define SAMP_FUNC_CURSORUNLOCKACTORCAM				0x9BC10
#define SAMP_FUNC_UPDATESCOREBOARDDATA				0x8A10
#define SAMP_FUNC_CNETGAMEDESTRUCTOR				0x9380
#define SAMP_FUNC_SENDINTERIOR						0x5740
#define SAMP_FUNC_DISABLE_SCOREBOARD				0x6A320

#define SAMP_HOOKENTER_STATECHANGE					0x12B82
#define SAMP_HOOKENTER_HANDLE_RPC					0x3743D
#define SAMP_HOOKENTER_HANDLE_RPC2					0x373C9
#define SAMP_HOOKENTER_CNETGAME_DESTR				0xB2893
#define SAMP_HOOKENTER_CNETGAME_DESTR2				0xB3A32
#define SAMP_HOOKEXIT_ANTICARJACK					0x12B93
#define SAMP_HOOKEXIT_HANDLE_RPC					0x37443
#define SAMP_HOOKEXIT_HANDLE_RPC2					0x37451

#define SAMP_PATCH_DISABLE_NAMETAGS					0x70D40
#define SAMP_PATCH_DISABLE_NAMETAGS_HP				0x6FC30
#define SAMP_PATCH_SKIPSENDINTERIOR					0x7668
#define SAMP_PATCH_SCOREBOARDTOGGLEON				0x6AA10
#define SAMP_PATCH_SCOREBOARDTOGGLEONKEYLOCK		0x6AD30
#define SAMP_PATCH_CHATINPUTADJUST_Y				0x63FE6
#define SAMP_PATCH_CHATINPUTADJUST_X				0x65855
#pragma endregion

#pragma region GTASTUFF
	struct pool
	{
#pragma pack( 1 )
		void	*start;		/* pointer to start of pool */
		void	*__end;		/* end of this structure? (not the pool) */
		int32_t size;		/* max number of items in the pool */
							/* there's some more junk after this */
	};

#pragma pack(push, 1)
	typedef struct _VECTOR
	{
		float	X, Y, Z;
	} VECTOR, *PVECTOR;

	typedef struct _MATRIX4X4
	{
		VECTOR	right;
		DWORD	flags;
		VECTOR	up;
		float	pad_u;
		VECTOR	at; // front
		float	pad_a;
		VECTOR	pos;
		float	pad_p;
	} MATRIX4X4, *PMATRIX4X4;

	struct detachable
	{
		float	position[3];
		uint8_t __unknown_12[4];
		float	vector[3];
		uint8_t __unknown_28[16];
	};

	struct weapon
	{
		uint32_t	id;
		uint32_t	state;
		int32_t		ammo_clip;
		int32_t		ammo;
		uint32_t	__unknown_count;
		uint32_t	__goggle;	/* 0 off, 256 on */
		uint32_t	__unknown;
	};

	// object_base is now unioned with a CEntity pointer where applicable
	struct object_base
	{
		void	*vtbl;	// 0

						// this is now in CEntity
						//	CPlaceableSAInterface	Placeable;	// 4
						// but the following variables exist for compatability till we remove object_base completely
		float					coords[3];			// 4
		union
		{
			// is MTA right?
			float			m_heading;				// 16

													// and are we wrong?
			void			*m_CMatrixPre;			// 16, a part of CPlaceable
			float			*preMatrix;				// 16, a part of CPlaceable
			MATRIX4X4		*preMatrixStruct;		// 16, a part of CPlaceable
		};
		union
		{
			void			*m_CMatrix;				// 20
			float			*matrix;				// 20
			MATRIX4X4		*matrixStruct;			// 20
		};

		void				*m_pRwObject;				// 24

														/********** BEGIN CFLAGS **************/
		unsigned long	bUsesCollision : 1;			// does entity use collision
		unsigned long	bCollisionProcessed : 1;	// has object been processed by a ProcessEntityCollision function
		unsigned long	bIsStatic : 1;			// is entity static
		unsigned long	bHasContacted : 1;		// has entity processed some contact forces
		unsigned long	bIsStuck : 1;			// is entity stuck
		unsigned long	bIsInSafePosition : 1;	// is entity in a collision free safe position
		unsigned long	bWasPostponed : 1;		// was entity control processing postponed
		unsigned long	bIsVisible : 1;			//is the entity visible
		unsigned long	bIsBIGBuilding : 1;		// Set if this entity is a big building
		unsigned long	bRenderDamaged : 1;		// use damaged LOD models for objects with applicable damage
		unsigned long	bStreamingDontDelete : 1;	// Dont let the streaming remove this
		unsigned long	bRemoveFromWorld : 1;		// remove this entity next time it should be processed
		unsigned long	bHasHitWall : 1;			// has collided with a building (changes subsequent collisions)
		unsigned long	bImBeingRendered : 1;		// don't delete me because I'm being rendered
		unsigned long	bDrawLast : 1;			// draw object last
		unsigned long	bDistanceFade : 1;		// Fade entity because it is far away
		unsigned long	bDontCastShadowsOn : 1; // Dont cast shadows on this object
		unsigned long	bOffscreen : 1;			// offscreen flag. This can only be trusted when it is set to true
		unsigned long	bIsStaticWaitingForCollision : 1;	// this is used by script created entities - they are static until the collision is loaded below them
		unsigned long	bDontStream : 1;			// tell the streaming not to stream me
		unsigned long	bUnderwater : 1;			// this object is underwater change drawing order
		unsigned long	bHasPreRenderEffects : 1;	// Object has a prerender effects attached to it
		unsigned long	bIsTempBuilding : 1;		// whether or not the building is temporary (i.e. can be created and deleted more than once)
		unsigned long	bDontUpdateHierarchy : 1;	// Don't update the aniamtion hierarchy this frame
		unsigned long	bHasRoadsignText : 1;		// entity is roadsign and has some 2deffect text stuff to be rendered
		unsigned long	bDisplayedSuperLowLOD : 1;
		unsigned long	bIsProcObject : 1;			// set object has been generate by procedural object generator
		unsigned long	bBackfaceCulled : 1;		// has backface culling on
		unsigned long	bLightObject : 1;			// light object with directional lights
		unsigned long	bUnimportantStream : 1;		// set that this object is unimportant, if streaming is having problems
		unsigned long	bTunnel : 1;			// Is this model part of a tunnel
		unsigned long	bTunnelTransition : 1;	// This model should be rendered from within and outside of the tunnel

												/********** END CFLAGS **************/
		uint8_t			wSeedColFlags;			// 32
		uint8_t			wSeedVisibleFlags;		// 33
		uint16_t		model_alt_id;			// 34 - model index
		uint8_t			__unknown_36[4];		// 36 - collision related (objects only?)

												//CReferences *pReferences; //36
		uint32_t		*m_pLastRenderedLink;	// 40 CLink<CEntity*>* m_pLastRenderedLink;
		uint16_t		timer;			// 44
		uint8_t			m_iplIndex;		// 46 used to define which IPL file object is in
		uint8_t			interior_id;	// 47
		uint8_t			__unknown_48[6];		// 48

												//********* BEGIN CEntityInfo **********//
		uint8_t			nType : 3;				// 54 what type is the entity (2 == Vehicle)
		uint8_t			nStatus : 5;			// 54 control status

												// 55 alignment
												//********* END CEntityInfo **********//
		uint8_t			__unknown_56[8];		// 56
		uint8_t			quantumPhysics;			// 64 // this really needs to be figured out cos it's probably a bit field
		uint8_t			nImmunities;			// 65
		uint8_t			__unknown_66;			// 66
	};

	struct actor_info
	{
		struct object_base	base;				/* 0-65 */
		uint8_t				flags;				/* 66 immunities */
		uint8_t				__unknown_67[1];	/* 67 */
		union	/* 68 */
		{
			float	speed[3];
			struct
			{
				D3DXVECTOR3 m_SpeedD3DVec;
			};
		};
		float				spin[3];			/* 80 */
		float				speed_rammed[3];	/* 92 */
		float				spin_rammed[3];		/* 104 */
		uint8_t				__unknown_116[60];	/* 116 */
		void				*__unknown_176;		/* 176 - pointer to a "entry node info" pool item */
		void				*__unknown_180;		/* 180 - pointer to a "ptr node Double" pool item */

												//collision data
		DWORD				collision_flags;	/* 184 - 2nd byte = currently colliding 1/0, or actively
												running against (0x2), 3rd byte = type colliding with
												(1 = road, 4=pavement, 35 = object, 3f=car).. unsure about 3rd byte

												nuck notes: 186 is used to tell if a surface is "steep" too,
												actually any slight angle at all
												*/
		void				*last_touched_object;	/* 188 - You can touch roads - those are considered buildings */
		void				*last_collided_object;	/* 192 - pointer to object last collided with (on foot, not jetpack) */
		uint8_t				__unknown_196[16];		/* 196 */
		float				speed_z;			/* 212 */
		float				collision_time_216; /* 216 - collision timer? */
		void				*collision_current_obj;		/* 220 - pointer to current thing colliding with */
		uint8_t				collision_something[12];	/* 224 - related to collision */
		float				collision_last_coord[3];	/* 236 - coordination of last collision */

														//end of collision data
		uint8_t				__unknown_248[100];			/* 248 */
														// 252 - animation related
		uint8_t				animation_state;			/* 348 */
		uint8_t				__unknown_349[7];			/* 349 */
		float				step_pos[3];		/* 356 - coordinates, last foot step */
		float				step_pos_before[3]; /* 368 - coordinates, foot step before last one */
		uint8_t				__unknown_380[752]; /* 380 */

		uint8_t			pedFlags[16];			/* 1132 */
		void *pPedIntelligence; /* 1148 */
		void  *pPlayerData; /* 1152 */

		uint8_t				__unknown_1156[8]; /* 1156 */

											   // relative body-part-positions
		void				*pNeck;				/* 1164 - void* pNeck
												(pChest?)
												- 4 unknown
												- float height
												- float front/back
												- float left/right
												- void* pObject
												(same struct for the following body-part-pointers [except pLeft/RightArm])
												*/
		void				*pHead;				/* 1168 */
		void				*pLeftArm;			/* 1172 */
		void				*pRightArm;			/* 1176 */
		void				*pLeftLowArm;		/* 1180 */
		void				*pRightLowArm;		/* 1184 */
		void				*pLeftLeg;			/* 1188 */
		void				*pRightLeg;			/* 1192 */
		void				*pLeftLowLeg;		/* 1196 */
		void				*pRightLowLeg;		/* 1200 */
		void				*pRightMidLeg;		/* 1204 */
		void				*pLeftMidLeg;		/* 1208 */
		void				*pLeftMidArm;		/* 1212 */
		void				*pRightMidArm;		/* 1216 */
		void				*pLeftShoulder;		/* 1220 */
		void				*pRightShoulder;	/* 1224 */
		void				*pChest;			/* 1228 */
		void				*pMouth;			/* 1232 */
												// end of body-part-positions

		uint8_t				runningStyle;		/* 1236 - 0x36 = cj, 0x8A = rollerskate,
												0x7F = woozie, 0x75 = crash.. etc. */
		uint8_t				__unknown_1237[7];	/* 1237 */
		float				runspeed;			/* 1244 */
		uint8_t				__unknown_1248[36]; /* 1248 */
		uint16_t			muzzle_flash;		/* 1284 */
		uint8_t				__unknown_1286[6];	/* 1286 */

		uint8_t				pedIK[32];	/* 1292 - Inverse Kinematics */
		uint8_t				__unknown_1324[4];	/* 1324 */

		uint32_t			state;	/* 1328 - ACTOR_STATE_* */
		uint32_t			mstate; /* 1332 - ACTOR_MSTATE_* */
		uint8_t				__unknown_1336[8];	/* 1336 */
		float				hitpoints;			/* 1344 */
		float				hitpoints_max;		/* 1348 - hmm, does not seem to be right.. it's set to "100.1318519" or something like that */
		float				armor;	/* 1352 */
		uint8_t				__unknown_1356[12];		/* 1356 */

													// 1360 and 1364 are using in resetting on-foot position when re-standing up

		float				fCurrentRotation;		/* 1368 */
		float				fTargetRotation;		/* 1372 */
		float				fRotationSpeed;			/* 1376 */
		float				fRotationCalculation;	/* 1380 - is 0.1 when target rotation reached */

		union
		{
			void	*pContactEntity;	/* 1384 - touching a CEntitySAInterface */
			struct vehicle_info *vehicle_contact;	/* 1384 - standing on top of vehicle */
		};

		float	vehicle_contact_dist[3];			/* 1388 - distance to the middle of the car standing on */
		float	fPlaneAnglesContactEntity[3];		/* 1400 - Angles of the plane of the contact entity the actor is
													standing on (?) */
		void	*item_contact;						/* 1412 - standing on top of vehicle/object/building/...*/
		float	fUnknown_1416;						/* 1416 - only changing when slightly under map */

		union
		{
			//CEntitySAInterface	*CurrentObjective;	/* 1420 - usually current vehicle's entity */
			struct vehicle_info *vehicle;
		};

		uint8_t				__unknown_1424[8];	/* 1424 */
		uint8_t				actor_lock;			/* 1432 */
		uint8_t				__unknown_1433[7];	/* 1433 */
		struct weapon		weapon[13];			/* 1440 */
		uint8_t				__unknown_1804[12]; /* 1804 */
		uint8_t				weapon_slot;		/* 1816 */
		uint8_t				__unknown_1817[3];	/* 1817 */
		void				*ptr_autoAimTarget;	/* 1820 - weapon target */
		float				fImpactPos_close[3];/* 1824 - last impact point of close range weapon [fist/knife/csaw/etc.] */
		uint8_t				__unknown_1836[20]; /* 1836 */
		uint32_t			weapon_model;		/* 1856 */
		uint8_t				__unknown_1860[28]; /* 1860 */
		uint32_t			weapon_hit_type;	/* 1888 - last hit by this weapon */
		struct actor_info	*weapon_hit_by;		/* 1892 - last hit by this object (usually an actor) */
		uint8_t				__unknown_1889[92]; /* 1896 */
	};	/* 1988 */

	struct vehicle_info
	{
		struct object_base			base;
		uint8_t						flags;			/* 66 - flags */
		uint8_t						__unknown_67;	/* 67 */
		float						speed[3];
		float						spin[3];
		float						speed_rammed[3];	/* 92 - speed from collision, will be added to speed[3] */
		float						spin_rammed[3];		/* 104 - spin from collision, will be added to spin[3] */

		uint8_t						__unknown_116[20];	/* 116 */

														// handling should start here
		uint8_t						__unknown_136[4];	/* 136 */
		float						mass;		/* 140 - vehicle mass from handling.cfg */
		float						turn_mass;	/* 144 - turn mass */
		float						grip_div;	/* 148 - grip divisor */
		float						mass_to_grip_mult;		/* 152 - mass to grip multiplier */
		float						fTurnMass;				/* 156 - 0.05 or 0.1 */
		float						grip_level_norm;		/* 160 - normalized grip level */
		float						center_of_mass[3];		/* 164 - center of mass */
		void						*__unknown_176;			/* 176 - pointer to a "entry node info" pool item */
		void						*__unknown_180;			/* 180 - pointer to a "ptr node Double" pool item */

															//collision data
		DWORD						collision_flags;		/* 184 - 2nd byte = currently colliding 1/0, or actively
															running against (0x02), being moved/rammed (0x03), 1st byte = 0, if noone inside and colliding
															with actor, else ever 0x0a*/
		void						*last_touched_object;	/* 188 - You can touch roads - those are considered buildings */
		void						*last_collided_object;	/* 192 - pointer to last collided object.. not for ALL collisions?!? */
		uint8_t						__unknown_196[20];		/* 196 */
		float						collision_something;	/* 216 - collision timer?*/
		void						*collision_current_obj; /* 220 - pointer to current thing colliding with */

															// end of handling should be here
		uint8_t						__unknown_224[12];		/* 224 */
		float						collision_position[3];	/* 236 - last collision coordinates */
		uint8_t						__unknown_248[68];		/* 248 */
		void						*__unknown_316;			/* 316 - pointer to this something in this structure */
		uint8_t						__unknown_320[157];		/* 320 */
		uint8_t						in_vehicle;			/* 477 */
		uint8_t						__unknown_478[422]; /* 478 */

		void						*pHandlingData;		/* 900 */
		struct physicalFlyParams	*pFlyData;			/* 904 */
		uint8_t						__unknown_908[96];	/* 908 */
		uint32_t					hFlagsLocal;		/* 1004 */
		uint8_t						__unknown_1008[56]; /* 1008 - AutoPilot */
		uint8_t						m_nVehicleFlags[8];	/* 1064 - 60 bits/flags */
		uint32_t					m_TimeOfCreation;	/* 1072 - GetTimeInMilliseconds when this vehicle was created. */

		uint8_t						color[4];			/* 1076 - as in carcolors.dat - body, stripe, body2, stripe2 */
		uint8_t						modding[2];			/* 1080 - modding data */
		unsigned short				UpgradeModelID[15];	/* 1082 - attached upgrades */
		float						wheel_size;			/* 1112 */
		unsigned short				CarAlarmState;		/* 1116 - time left for car alarm to sound in ms */
		unsigned short				ForcedRandomSeed;	/* 1118 - if this is non-zero the random wander gets deterministic */
		struct actor_info			*passengers[9];		/* 1120 (pointer to the passenger player structs) */

		unsigned char				m_nNumPassengers;	/* 1156 */
		unsigned char				m_nNumGettingIn;	/* 1157 */
		unsigned char				m_nGettingInFlags;	/* 1158 */
		unsigned char				m_nGettingOutFlags; /* 1159 */
		unsigned char				m_nMaxPassengers;	/* 1160 */
		unsigned char				m_windowsOpenFlags; /* 1161 */
		char						m_nNitroBoosts;		/* 1162 */
		char						m_nSpecialColModel; /* 1163 */
		DWORD						*pEntityWeAreOnForVisibilityCheck;	/* 1164 - we need a CEntity */
		DWORD						*m_pFire;			/* 1168 - CFire*/

		float						steer_angles[2];	/* 1172 - steer angles */
		float						gas_pedal;			/* 1180 - gas pedal */
		float						break_pedal;		/* 1184 - break pedal */

														// 1188
		unsigned char				VehicleCreatedBy;	// Contains information on whether this vehicle can be deleted or not. Probably only need this or IsLocked.
		short						ExtendedRemovalRange;

		// this padding probably isn't in the right place.
		uint8_t						__unknown_1191;

		//1192
		unsigned char				car_bomb : 3;		// 0 = None. 1 = Timed. 2 = On ignition, 3 = remotely set ? 4 = Timed Bomb has been activated. 5 = On ignition has been activated.
		unsigned char				OverrideLights : 2; // uses enum NO_CAR_LIGHT_OVERRIDE, FORCE_CAR_LIGHTS_OFF, FORCE_CAR_LIGHTS_ON
		unsigned char				WinchType : 2;		// Does this vehicle use a winch?

														// this padding probably isn't in the right place.
		uint8_t						__unknown_1193;

		//1194
		unsigned char				m_GunsCycleIndex : 2;		// Cycle through alternate gun hardpoints on planes/helis
		unsigned char				m_OrdnanceCycleIndex : 2;	// Cycle through alternate ordnance hardpoints on planes/helis

																// 1195
		unsigned char				nUsedForCover;			// Has n number of cops hiding/attempting to hid behind it
		unsigned char				AmmoInClip;				// Used to make the guns on boat do a reload.
		unsigned char				PacMansCollected;
		unsigned char				PedsPositionForRoadBlock;
		unsigned char				NumPedsForRoadBlock;

		//1200
		float						nBodyDirtLevel;			// Dirt level of vehicle body texture: 0.0f=fully clean, 15.0f=maximum dirt visible, it may be altered at any time while vehicle's cycle of lige

															// values used by transmission
		unsigned char				m_nCurrentGear;			/* 1204 */
		float						m_fGearChangeCount;		/* 1205 */
		float						m_fWheelSpinForAudio;	/* 1209 */
		uint8_t						__unknown_1213[3];		/* 1213 */

		float						hitpoints;			/* 1216 */
		void						*pulling_truck;		/* 1220 - vehicle_info - pulling truck */
		struct vehicle_info			*trailer;			/* 1224 - pointer to the currently attached trailer; 0 if theres no trailer */
		uint8_t						__unknown_1228[44]; /* 1228 */
		uint32_t					door_status;		/* 1272 - car door status (1:Opened 2:Locked) */
		uint8_t						__unknown_1276[148];	/* 1276 */
		uint8_t						vehicle_type;			/* 1424 - VEHICLE_TYPE_* */
		uint8_t						__unknown_1425[19];		/* 1425 */

		union
		{
			struct
			{
				float	m_fTrainSpeed;			/* 1444 - Train speed along rails */
				float	m_fTrainRailDistance;	/* 1448 - Distance along rail starting from first rail node (determines train position when on rails) -max 18106 */
				float	m_fDistanceToNextCarriage;		/* 1452 - Distance to Carriage being infront of this */
			};
			struct
			{
				uint8_t boat_sound_level;				/* 1444 */
				uint8_t car_tire_status[4];				/* 1445 - LF, LR, RF, RR, 0 = ok, 1 = flat, 2 = landing gear up */
				uint8_t door_damage_status[6];			/* 1449 - damage status of doors, 0 = ok, 2 = damaged, 3 = open free, 4 = lost */
			};
		};

		// light damage states
		//uint8_t			lights;						/* 1456 - Light status, 0 = no dmg, 1 = front left dmg, 4 = front right dmg, 5 = front dmg, ...+0x40 = ...+back dmg */
		unsigned char		lights_status_frontLeft : 1;
		unsigned char		__unknown_lights_status_2 : 1;
		unsigned char		lights_status_frontRight : 1;
		unsigned char		__unknown_lights_status_4 : 3;
		unsigned char		lights_status_rear : 1;

		// end of light damage states
		uint8_t				__unknown_1457[3];			/* 1457 - 1475 related to doorHood (byte??)*/

														// misc damage states
		unsigned char		car_status_left : 4;		/* 1460 - 0 = ok, 1 = damaged, 2 = free movement, 3 = lost - same status codes for following bumper/windshield */
		unsigned char		car_status_right : 4;
		uint8_t				car_bumper_status;			/* 1461 - unknown */
		unsigned char		car_windshield_status : 4;
		unsigned char		car_bumper_status_front : 4;
		uint8_t				car_bumper_status_rear;
		// end of misc damage states

		uint8_t				m_trainFlags[4];			/* 1464 */
		uint8_t				__unknown_1468[8];			/* 1468 */
		float				doorHood_dist;				/* 1476 - positive = open */
		uint8_t				__unknown_1480[4];			/* 1480 - detachable related*/
		float				doorHood_dist_two;			/* 1484 - positive = open*/
		uint8_t				__unknown_1488[4];			/* 1488 */
		struct vehicle_info *m_train_next_carriage;		/* 1492 */
		uint8_t				__unknown_1496[4];			/* 1496 */
		float				doorTrunk_dist;				/* 1500 - negative = open */
		uint8_t				__unknown_1504[4];			/* 1504 */
		float				doorTrunk_dist_two;			/* 1508 - negative = open */
		uint8_t				__unknown_1512[12];			/* 1512 */
		float				doorOne_dist;				/* 1524 - also for movable part of boats (marquis)*/
		uint8_t				__unknown_1528[4];			/* 1528 */
		union
		{
			struct detachable	detachable_bike1[1];	/* 1532 - bike/motorcycle/car part */
			struct
			{
				float	doorOne_dist_two;			/* 1532 - negative = open */
				uint8_t __unknown_1536[12];			/* 1536 */
				float	doorTwo_dist;				/* 1548 - positive = open */
				uint8_t __unknown_1552[4];			/* 1552 */
				float	doorTwo_dist_two;			/* 1556  - positive = open */
				uint8_t __unknown_1560[12];			/* 1560 */
				float	doorThree_dist;				/* 1572 - negative = open */
				uint8_t __unknown_1576[4];			/* 1576 */
				float	doorThree_dist_two;			/* 1580 - negative = open */
				uint8_t __unknown_1584[12];			/* 1584 */
				float	doorFour_dist;				/* 1596 - positive = open */
				uint8_t __unknown_1600[6];			/* 1600 */
				float	doorFour_dist_two;			/* 1606 - positive = open */
			};
		};
		uint8_t				__unknown_1610[18];		/* 1610 */
		uint8_t				bike_tire_status[2];	/* 1628 Front, Rear, 0 = ok, 1 = flat */
		uint8_t				__unknown_1630[2];		/* 1630 */

													//maybe also door positions in here, like in 15xx?
		struct detachable	detachable_bike2[4];	/* 1632 - bike/motorcycle and boat parts */
		uint8_t				__unknown_1808[8];		/* 1808 */
		float				fPos_FireCarLadder[3];	/* 1816 - train ladder positioning (part of detachables?) */
		struct detachable	detachable_car[4];		/* 1828 - car/helicopter/airplane parts */
		uint8_t				__unknown_2004[12];		/* 2004 */
		float				suspension[4];			/* 2016 - FL, RL, FR, RR suspension height */
		uint8_t				__unknown_2032[244];	/* 2032 */
		float				burn_timer;				/* 2276 - burn timer counting up from 0.0f */
		uint8_t				__unknown_2280[100];	/* 2280 */
		float				fCannonAimX_Y[2];		/* 2380 - cannon offset (x/y) */
		uint8_t				__unknown_2388[48];		/* 2388 */
		float				fHeliDistanceToGround;	/* 2436 - (between 0.9 and 1 = touching ground) */
		uint8_t				__unknown_2440[24];		/* 2440 */
		float				fHeliMainRotorDistance;	/* 2464 - distance/rotation */
		float				fHeliSecRotorDistance;	/* 2468 - distance/rotation secondary rotor */
		uint8_t				__unknown_2472[88];		/* 2472 */
		uint8_t				SmokeTrailEnabled;		/* 2560 */
		uint8_t				__unknown_2561[23];		/* 2561 */
													/* 2584 */
	};

	struct object_info
	{
		struct object_base	base;
		uint8_t				__unknown_66;			/* 66 */
		uint8_t				flags;	/* 67 - collision and ...? */
		uint8_t				__unknown_68[7];	/* 68 */
		float				physical_height;	/* 75 - normally max. 0.2 (bigger value = more 'ouch' when jumping), different results for different objects? */
		uint8_t				__unknown_79[173];	/* 79 - first value changing when last movement was going up or going down*/
		struct actor_info	*actor; /* 252 - pointer to Actor Object is attached to, 0 when not attached */
		uint8_t				__unknown_256[8];	/* 256 */
		float				height_to_actor;	/* 264 - when attached height difference to actor, else 0*/
		uint8_t				__unknown_268[4];	/* 268 */
		float				pitch;	/* 272 - rotation, pitch (attached) */
		float				roll;	/* 276 - rotation, roll (attached) */
		uint8_t				__unknown_280[68];	/* 280 */
		float				scale;	/* 348 - Scale */
		uint8_t				__unknown_352[59];	/* 352 */
	};

	//Only for flying
	struct physicalFlyParams
	{
		float	__itsNull;	/* 0 */
		float	heightIncreaseRel[2];	/* 4 - Related to (Heli only?) height */
		float	circleAround;			/* 12 - Heli/Plane */
		float	changeDir;		/* 16 - Speed changing Direction (negative = change against the speed vector) */
		float	changeDir_side; /* 20 */
		float	roll_lr;		/* 24 */
		float	__unknown_28;	/* 28 */
		float	pitch;			/* 32 */
		float	__unknown_36;	/* 36 */
		float	_multPitch;		/* 40 - pitch related */
		float	finalPitch_;	/* 44 */
		float	_unknownMultipliers_isOne[2];	/* 48 */
		float	_orientationMultiplier;			/* 56 - somehow orientation related */
		float	_SpeedMultiplier_isOne;			/* 60 */
		float	pitchMultiplier;	/* 64 - keep it >0 */
		float	rollMultiplier;		/* 68 */
		float	circleMultiplier;	/* 72 */
									//following values = global for all vehs (?)
	};

	struct checkpoint
	{
		uint32_t	__unknown1;		/* Dunno, but I'm guessing the type of checkpoint is stored here */
		uint32_t	counter;		/* Some sort of counter. */
		uint32_t	color;			/* RGBA color value */
		uint32_t	__unknown2;		/* 1024 most of the time */
		float		position[3];
		float		rotation[3];	/* direction from this checkpoint to the next */
		float		zero_point_two;
		float		radius;			/* radius? */
		uint32_t	__unknown3;		/* always zero */
		uint32_t	__unknown4;		/* always zero */
	};

	struct vehicle_state
	{
		float					matrix[16];
		float					speed[3];
		float					spin[3];
		struct vehicle_state	*trailer;
		struct vehicle_info		*trailerPtr;
	};

#pragma endregion

#pragma region SAMPSTUFF
	enum DialogStyle
	{
		DIALOG_STYLE_MSGBOX,
		DIALOG_STYLE_INPUT,
		DIALOG_STYLE_LIST,
		DIALOG_STYLE_PASSWORD,
	};

	enum Limits
	{
		SAMP_MAX_ACTORS = 1000,
		SAMP_MAX_PLAYERS = 1004,
		SAMP_MAX_VEHICLES = 2000,
		SAMP_MAX_PICKUPS = 4096,
		SAMP_MAX_OBJECTS = 1000,
		SAMP_MAX_GANGZONES = 1024,
		SAMP_MAX_3DTEXTS = 2048,
		SAMP_MAX_TEXTDRAWS = 2048,
		SAMP_MAX_PLAYERTEXTDRAWS = 256,
		SAMP_MAX_CLIENTCMDS = 144,
		SAMP_MAX_MENUS = 128,
		SAMP_MAX_PLAYER_NAME = 24,
		SAMP_ALLOWED_PLAYER_NAME_LENGTH = 20,
	};

	enum ChatMessageType
	{
		CHAT_TYPE_NONE = 0,
		CHAT_TYPE_CHAT = 2,
		CHAT_TYPE_INFO = 4,
		CHAT_TYPE_DEBUG = 8
	};

	enum ChatDisplayMode
	{
		CHAT_WINDOW_MODE_OFF = 0,
		CHAT_WINDOW_MODE_LIGHT = 1,
		CHAT_WINDOW_MODE_FULL = 2
	};

	enum Gamestate
	{
		GAMESTATE_WAIT_CONNECT = 9,
		GAMESTATE_CONNECTING = 13,
		GAMESTATE_AWAIT_JOIN = 15,
		GAMESTATE_CONNECTED = 14,
		GAMESTATE_RESTARTING = 18
	};

	enum PlayerState
	{
		PLAYER_STATE_NONE = 0,
		PLAYER_STATE_ONFOOT = 17,
		PLAYER_STATE_PASSENGER,
		PLAYER_STATE_DRIVER,
		PLAYER_STATE_WASTED = 32,
		PLAYER_STATE_SPAWNED
	};

	enum MarkersMode
	{
		PLAYER_MARKERS_MODE_OFF,
		PLAYER_MARKERS_MODE_GLOBAL,
		PLAYER_MARKERS_MODE_STREAMED,
	};

	enum SpecialAction
	{
		SPECIAL_ACTION_NONE,
		SPECIAL_ACTION_DUCK,
		SPECIAL_ACTION_USEJETPACK,
		SPECIAL_ACTION_ENTER_VEHICLE,
		SPECIAL_ACTION_EXIT_VEHICLE,
		SPECIAL_ACTION_DANCE1,
		SPECIAL_ACTION_DANCE2,
		SPECIAL_ACTION_DANCE3,
		SPECIAL_ACTION_DANCE4,
		SPECIAL_ACTION_HANDSUP,
		SPECIAL_ACTION_USECELLPHONE,
		SPECIAL_ACTION_SITTING,
		SPECIAL_ACTION_STOPUSECELLPHONE,
		SPECIAL_ACTION_DRINK_BEER = 20,
		SPECIAL_ACTION_SMOKE_CIGGY,
		SPECIAL_ACTION_DRINK_WINE,
		SPECIAL_ACTION_DRINK_SPRUNK,
		SPECIAL_ACTION_CUFFED,
		SPECIAL_ACTION_CARRY,
	};


	//////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////// SAMP STRUCTURES ///////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)
	struct stSAMPPools
	{
		struct stActorPool		*pActor;
		struct stObjectPool		*pObject;
		struct stGangzonePool	*pGangzone;
		struct stTextLabelPool	*pText3D;
		struct stTextdrawPool	*pTextdraw;
		void					*pPlayerLabels;
		struct stPlayerPool		*pPlayer;
		struct stVehiclePool	*pVehicle;
		struct stPickupPool		*pPickup;
	};

	struct stSAMP
	{
		void					*pUnk0;
		struct stServerInfo			*pServerInfo;
		uint8_t					byteSpace[24];
		char					szIP[257];
		char					szHostname[259];
		bool					bNametagStatus; // changes by /nametagstatus
		uint32_t				ulPort;
		uint32_t				ulMapIcons[100];
		int						iLanMode;
		int						iGameState;
		uint32_t				ulConnectTick;
		struct stServerPresets	*pSettings;
		void					*pRakClientInterface;
		struct stSAMPPools		*pPools;
	};

	struct stServerInfo {
		uint32_t 			uiIP;
		uint16_t 			usPort;

		// ...

	};

	struct stServerPresets
	{
		uint8_t byteCJWalk;
		int m_iDeathDropMoney;
		float	fWorldBoundaries[4];
		bool m_bAllowWeapons;
		float	fGravity;
		uint8_t byteDisableInteriorEnterExits;
		uint32_t ulVehicleFriendlyFire;
		bool m_byteHoldTime;
		bool m_bInstagib;
		bool m_bZoneNames;
		bool m_byteFriendlyFire;
		int		iClassesAvailable;
		float	fNameTagsDistance;
		bool m_bManualVehicleEngineAndLight;
		uint8_t byteWorldTime_Hour;
		uint8_t byteWorldTime_Minute;
		uint8_t byteWeather;
		uint8_t byteNoNametagsBehindWalls;
		int iPlayerMarkersMode;
		float	fGlobalChatRadiusLimit;
		uint8_t byteShowNameTags;
		bool m_bLimitGlobalChatRadius;
	};



	struct stDialogInfo
	{
		DWORD	m_pD3DDevice;
		int	iTextPoxX;
		int	iTextPoxY;
		uint32_t	uiDialogSizeX;
		uint32_t	uiDialogSizeY;
		int	iBtnOffsetX;
		int	iBtnOffsetY;
		class _CDXUTDialog						*pDialog;
		class _CDXUTListBox						*pList;
		class _CDXUTIMEEditBox						*pEditBox;
		int	iIsActive;
		int	iType;
		uint32_t	DialogID;
		char		*pText;
		uint32_t	uiTextWidth;
		uint32_t	uiTextHeight;
		char		szCaption[65];
		int		bServerside;
	};

	struct stTextDrawTransmit
	{
		union
		{
			BYTE byteFlags;
			struct
			{
				BYTE byteBox : 1;
				BYTE byteLeft : 1;
				BYTE byteRight : 1;
				BYTE byteCenter : 1;
				BYTE byteProportional : 1;
				BYTE bytePadding : 3;
			};
		};
		float fLetterWidth;
		float fLetterHeight;
		DWORD dwLetterColor;
		float fBoxWidth;
		float fBoxHeight;
		DWORD dwBoxColor;
		BYTE byteShadow;
		BYTE byteOutline;
		DWORD dwBackgroundColor;
		BYTE byteStyle;
		BYTE byteUNK;
		float fX;
		float fY;
		uint16_t sModel;
		float fRot[3];
		float fZoom;
		WORD sColor[2];
	};

	struct stTextdraw
	{
		char		szText[800 + 1];
		char		szString[1600 + 2];
		float		fLetterWidth;
		float		fLetterHeight;
		DWORD		dwLetterColor;
		uint8_t		byte_unk;	// always = 01 (?)
		BYTE		byteCenter;
		BYTE		byteBox;
		float		fBoxSizeX;
		float		fBoxSizeY;
		DWORD		dwBoxColor;
		BYTE		byteProportional;
		DWORD		dwShadowColor;
		BYTE		byteShadowSize;
		BYTE		byteOutline;
		BYTE		byteLeft;
		BYTE		byteRight;
		int			iStyle;		// font style/texture/model
		float		fX;
		float		fY;
		byte		unk[8];
		DWORD		dword99B;	// -1 by default
		DWORD		dword99F;	// -1 by default
		DWORD		index;		// -1 if bad
		BYTE		byte9A7;	// = 1; 0 by default
		uint16_t	sModel;
		float		fRot[3];
		float		fZoom;
		WORD		sColor[2];
		BYTE		f9BE;
		BYTE		byte9BF;
		BYTE		byte9C0;
		DWORD		dword9C1;
		DWORD		dword9C5;
		DWORD		dword9C9;
		DWORD		dword9CD;
		BYTE		byte9D1;
		DWORD		dword9D2;
	};

	struct stTextdrawPool
	{
		int					iIsListed[SAMP_MAX_TEXTDRAWS];
		int					iPlayerTextDraw[SAMP_MAX_PLAYERTEXTDRAWS];
		struct stTextdraw	*textdraw[SAMP_MAX_TEXTDRAWS];
		struct stTextdraw	*playerTextdraw[SAMP_MAX_PLAYERTEXTDRAWS];
	};

	struct stPickup
	{
		int		iModelID;
		int		iType;
		float	fPosition[3];
	};

	struct stPickupPool
	{
		int				iPickupsCount;
		uint32_t		ul_GTA_PickupID[SAMP_MAX_PICKUPS];
		int				iPickupID[SAMP_MAX_PICKUPS];
		int				iTimePickup[SAMP_MAX_PICKUPS];
		uint8_t			unk[SAMP_MAX_PICKUPS * 3];
		struct stPickup pickup[SAMP_MAX_PICKUPS];
	};

	struct stPlayerPool
	{
		uint32_t				ulMaxPlayerID;
		uint16_t				sLocalPlayerID;
		void					*pVTBL_txtHandler;
		std::string				strLocalPlayerName;
		struct stLocalPlayer	*pLocalPlayer;
		int						iLocalPlayerPing;
		int						iLocalPlayerScore;
		struct stRemotePlayer	*pRemotePlayer[SAMP_MAX_PLAYERS];
		int						iIsListed[SAMP_MAX_PLAYERS];
		DWORD						dwPlayerIP[SAMP_MAX_PLAYERS]; // always 0
	};

	struct stSAMPKeys
	{
		uint8_t keys_primaryFire : 1;
		uint8_t keys_horn__crouch : 1;
		uint8_t keys_secondaryFire__shoot : 1;
		uint8_t keys_accel__zoomOut : 1;
		uint8_t keys_enterExitCar : 1;
		uint8_t keys_decel__jump : 1;			// on foot: jump or zoom in
		uint8_t keys_circleRight : 1;
		uint8_t keys_aim : 1;					// hydra auto aim or on foot aim
		uint8_t keys_circleLeft : 1;
		uint8_t keys_landingGear__lookback : 1;
		uint8_t keys_unknown__walkSlow : 1;
		uint8_t keys_specialCtrlUp : 1;
		uint8_t keys_specialCtrlDown : 1;
		uint8_t keys_specialCtrlLeft : 1;
		uint8_t keys_specialCtrlRight : 1;
		uint8_t keys__unused : 1;
	};

	struct stOnFootData
	{
		uint16_t	sLeftRightKeys;
		uint16_t	sUpDownKeys;
		union
		{
			uint16_t			sKeys;
			struct stSAMPKeys	stSampKeys;
		};
		float		fPosition[3];
		float		fQuaternion[4];
		uint8_t		byteHealth;
		uint8_t		byteArmor;
		uint8_t		byteCurrentWeapon;
		uint8_t		byteSpecialAction;
		float		fMoveSpeed[3];
		float		fSurfingOffsets[3];
		uint16_t	sSurfingVehicleID;
		short		sCurrentAnimationID;
		short		sAnimFlags;
	};

	struct stInCarData
	{
		uint16_t	sVehicleID;
		uint16_t	sLeftRightKeys;
		uint16_t	sUpDownKeys;
		union
		{
			uint16_t			sKeys;
			struct stSAMPKeys	stSampKeys;
		};
		float		fQuaternion[4];
		float		fPosition[3];
		float		fMoveSpeed[3];
		float		fVehicleHealth;
		uint8_t		bytePlayerHealth;
		uint8_t		byteArmor;
		uint8_t		byteCurrentWeapon;
		uint8_t		byteSiren;
		uint8_t		byteLandingGearState;
		uint16_t	sTrailerID;
		union
		{
			uint16_t	HydraThrustAngle[2];	//nearly same value
			float		fTrainSpeed;
		};
	};

	struct stAimData
	{
		BYTE	byteCamMode;
		float	vecAimf1[3];
		float	vecAimPos[3];
		float	fAimZ;
		BYTE	byteCamExtZoom : 6;		// 0-63 normalized
		BYTE	byteWeaponState : 2;	// see eWeaponState
		BYTE	bUnk;
	};

	struct stTrailerData
	{
		uint16_t	sTrailerID;
		float		fPosition[3];
		//float		fRoll[3];
		//float		fDirection[3];
		float		fQuaternion[4]; // not tested
		float		fSpeed[3];
		float		fTurnSpeed[3];
		//	float		fUnk[2];
		//	uint32_t	pad;
	};

	struct stPassengerData
	{
		uint16_t	sVehicleID;
		uint8_t		byteSeatID;
		uint8_t		byteCurrentWeapon;
		uint8_t		byteHealth;
		uint8_t		byteArmor;
		uint16_t	sLeftRightKeys;
		uint16_t	sUpDownKeys;
		union
		{
			uint16_t			sKeys;
			struct stSAMPKeys	stSampKeys;
		};
		float	fPosition[3];
	};

	struct stDamageData
	{
		uint16_t	sVehicleID_lastDamageProcessed;
		int			iBumperDamage;
		int			iDoorDamage;
		uint8_t		byteLightDamage;
		uint8_t		byteWheelDamage;
	};

	struct stSurfData
	{
		int			iIsSurfing;
		float		fSurfPosition[3];
		int			iUnk0;
		uint16_t	sSurfingVehicleID;
		uint32_t	ulSurfTick;
		struct stSAMPVehicle *pSurfingVehicle;
		int			iUnk1;
		int			iSurfMode;	//0 = not surfing, 1 = moving (unstable surf), 2 = fixed on vehicle
	};

	struct stUnoccupiedData
	{
		int16_t sVehicleID;
		uint8_t byteSeatID;
		float	fRoll[3];
		float	fDirection[3];
		float	fPosition[3];
		float	fMoveSpeed[3];
		float	fTurnSpeed[3];
		float	fHealth;
	};

	struct stBulletData
	{
		uint8_t		byteType;
		uint16_t	sTargetID;
		float		fOrigin[3];
		float		fTarget[3];
		float		fCenter[3];
		uint8_t		byteWeaponID;
	};

	struct stSpectatorData
	{
		uint16_t	sLeftRightKeys;
		uint16_t	sUpDownKeys;
		union
		{
			uint16_t			sKeys;
			struct stSAMPKeys	stSampKeys;
		};
		float	fPosition[3];
	};

	struct stStatsData
	{
		int iMoney;
		int iAmmo;	// ?
	};

	struct stHeadSync
	{
		float	fHeadSync[3];
		int		iHeadSyncUpdateTick;
		int		iHeadSyncLookTick;
	};

	struct stLocalPlayer
	{
		struct stSAMPPed		*pSAMP_Actor;
		uint16_t				sCurrentAnimID;
		uint16_t				sAnimFlags;
		uint32_t				ulUnk0;
		int						iIsActive;
		int						iIsWasted;
		uint16_t				sCurrentVehicleID;
		uint16_t				sLastVehicleID;
		struct stOnFootData		onFootData;
		struct stPassengerData	passengerData;
		struct stTrailerData	trailerData;
		struct stInCarData		inCarData;
		struct stAimData		aimData;
		uint8_t					byteTeamID;
		int						iSpawnSkin;
		uint8_t					byteUnk1;
		float					fSpawnPos[3];
		float					fSpawnRot;
		int						iSpawnWeapon[3];
		int						iSpawnAmmo[3];
		int						iIsActorAlive;
		int						iSpawnClassLoaded;
		uint32_t				ulSpawnSelectionTick;
		uint32_t				ulSpawnSelectionStart;
		int						iIsSpectating;
		uint8_t					byteTeamID2;
		uint16_t				usUnk2;
		uint32_t				ulSendTick;
		uint32_t				ulSpectateTick;
		uint32_t				ulAimTick;
		uint32_t				ulStatsUpdateTick;
		uint32_t				ulWeapUpdateTick;
		uint16_t				sAimingAtPid;
		uint16_t				usUnk3;
		uint8_t					byteCurrentWeapon;
		uint8_t					byteWeaponInventory[13];
		int						iWeaponAmmo[13];
		int						iPassengerDriveBy;
		uint8_t					byteCurrentInterior;
		int						iIsInRCVehicle;
		uint16_t				sTargetObjectID;
		uint16_t				sTargetVehicleID;
		uint16_t				sTargetPlayerID;
		struct stHeadSync		headSyncData;
		uint32_t				ulHeadSyncTick;
		BYTE					byteSpace3[260];
		struct stSurfData		surfData;
		int						iClassSelectionOnDeath;
		int						iSpawnClassID;
		int						iRequestToSpawn;
		int						iIsInSpawnScreen;
		uint32_t				ulUnk4;
		uint8_t					byteSpectateMode;		// 3 = vehicle, 4 = player, side = 14, fixed = 15
		uint8_t					byteSpectateType;		// 0 = none, 1 = player, 2 = vehicle
		int						iSpectateID;
		int						iInitiatedSpectating;
		struct stDamageData		vehicleDamageData;
	};

	struct stRemotePlayerData
	{
		struct stSAMPPed		*pSAMP_Actor;
		struct stSAMPVehicle	*pSAMP_Vehicle;
		uint8_t					byteTeamID;
		uint8_t					bytePlayerState;
		uint8_t					byteSeatID;
		uint32_t				ulUnk3;
		int						iPassengerDriveBy;
		void					*pUnk0;
		uint8_t					byteUnk1[60];
		float					fSomething[3];
		float					fVehicleRoll[4];
		uint32_t				ulUnk2[3];
		float					fOnFootPos[3];
		float					fOnFootMoveSpeed[3];
		float					fVehiclePosition[3];
		float					fVehicleMoveSpeed[3];
		uint16_t				sPlayerID;
		uint16_t				sVehicleID;
		uint32_t				ulUnk5;
		int						iShowNameTag;
		int						iHasJetPack;
		uint8_t					byteSpecialAction;
		uint32_t				ulUnk4[3];
		struct stOnFootData		onFootData;
		struct stInCarData		inCarData;
		struct stTrailerData	trailerData;
		struct stPassengerData	passengerData;
		struct stAimData		aimData;
		float					fActorArmor;
		float					fActorHealth;
		uint32_t				ulUnk10;
		uint8_t					byteUnk9;
		uint32_t				dwTick;
		uint32_t				dwLastStreamedInTick;	// is 0 when currently streamed in
		uint32_t				ulUnk7;
		int						iAFKState;
		struct stHeadSync		headSyncData;
		int						iGlobalMarkerLoaded;
		int						iGlobalMarkerLocation[3];
		uint32_t				ulGlobalMarker_GTAID;
		//int						lastBulletTime;
	};

	struct stRemotePlayer
	{
		stRemotePlayerData	*pPlayerData;
		int					iIsNPC;
		void				*pVTBL_txtHandler;
		std::string			strPlayerName;
		int					iScore;
		int					iPing;
		int					iNameLen;
		int					iNameAllocated;
		union
		{
			char			szPlayerName[16];
			char			*pszPlayerName;
		};
	};

	template <typename T>
	struct stSAMPEntity
	{
		void		*pVTBL; //я ебу указатель кароче и там +- через 40 байт позиция ебашит
		uint8_t		byteUnk0[60]; // game CEntity object maybe. always empty.
		T			*pGTAEntity;
		uint32_t	ulGTAEntityHandle;
	};

	struct stSAMPPed : public stSAMPEntity < actor_info >
	{
		int					usingCellPhone;
		uint8_t				byteUnk0[600];
		struct actor_info	*pGTA_Ped;
		uint8_t				byteUnk1[22];
		uint8_t				byteKeysId;
		uint16_t			ulGTA_UrinateParticle_ID;
		int					DrinkingOrSmoking;
		int					object_in_hand;
		int					drunkLevel;
		uint8_t				byteUnk2[5];
		int					isDancing;
		int					danceStyle;
		int					danceMove;
		uint8_t				byteUnk3[20];
		int					isUrinating;
	};

	struct stVehiclePool
	{
		int						iVehicleCount;
		void					*pUnk0;
		uint8_t					byteSpace1[0x112C];
		struct stSAMPVehicle	*pSAMP_Vehicle[SAMP_MAX_VEHICLES];
		int						iIsListed[SAMP_MAX_VEHICLES];
		struct vehicle_info		*pGTA_Vehicle[SAMP_MAX_VEHICLES];
		uint8_t					byteSpace2[SAMP_MAX_VEHICLES * 6];
		uint32_t				ulShit[SAMP_MAX_VEHICLES];
		int						iIsListed2[SAMP_MAX_VEHICLES];
		uint32_t				byteSpace3[SAMP_MAX_VEHICLES * 2];
		float					fSpawnPos[SAMP_MAX_VEHICLES][3];
		int						iInitiated;
	};

	struct stSAMPVehicle : public stSAMPEntity < vehicle_info >
	{
		uint32_t			bUnk0;
		struct vehicle_info *pGTA_Vehicle;
		uint8_t				byteUnk1[8];
		int					bIsMotorOn;
		int					iIsLightsOn;
		int					iIsLocked;
		uint8_t				byteIsObjective;
		int					iObjectiveBlipCreated;
		uint8_t				byteUnk2[16];
		uint8_t				byteColor[2];
		int					iColorSync;
		int					iColor_something;
	};

	struct stObject : public stSAMPEntity < object_info >
	{
		uint8_t				byteUnk0[2];
		uint32_t			ulUnk1;
		int					iModel;
		uint16_t			byteUnk2;
		float				fDrawDistance;
		float				fUnk;
		float				fPos[3];
		uint8_t				byteUnk3[68];
		uint8_t				byteUnk4;
		float				fRot[3];
		// ...
	};

	struct stObjectPool
	{
		int				iObjectCount;
		int				iIsListed[SAMP_MAX_OBJECTS];
		struct stObject *object[SAMP_MAX_OBJECTS];
	};

	struct stGangzone
	{
		float	fPosition[4];
		DWORD	dwColor;
		DWORD	dwAltColor;
	};

	struct stGangzonePool
	{
		struct stGangzone	*pGangzone[SAMP_MAX_GANGZONES];
		int					iIsListed[SAMP_MAX_GANGZONES];
	};

	struct stTextLabel
	{
		char		*pText;
		DWORD		color;
		float		fPosition[3];
		float		fMaxViewDistance;
		uint8_t		byteShowBehindWalls;
		uint16_t	sAttachedToPlayerID;
		uint16_t	sAttachedToVehicleID;
	};

	struct stTextLabelPool
	{
		struct stTextLabel	textLabel[SAMP_MAX_3DTEXTS];
		int					iIsListed[SAMP_MAX_3DTEXTS];
	};

	struct stChatEntry
	{
		uint32_t	SystemTime;
		char		szPrefix[28];
		char		szText[144];
		uint8_t		unknown[64];
		int			iType;			// 2 - text + prefix, 4 - text (server msg), 8 - text (debug)
		D3DCOLOR	clTextColor;
		D3DCOLOR	clPrefixColor;	// or textOnly colour
	};

	struct stFontRenderer
	{
		ID3DXFont	*m_pChatFont;
		ID3DXFont	*m_pLittleFont;
		ID3DXFont	*m_pChatShadowFont;
		ID3DXFont	*m_pLittleShadowFont;
		ID3DXFont	*m_pCarNumberFont;
		ID3DXSprite *m_pTempSprite;
		IDirect3DDevice9		*m_pD3DDevice;
		char		*m_pszTextBuffer;
	};

	struct stChatInfo
	{
		int					pagesize;
		char				*pLastMsgText;
		int					iChatWindowMode;
		uint8_t				bTimestamps;
		uint32_t			m_iLogFileExist;
		char				logFilePathChatLog[MAX_PATH + 1];
		void				*pGameUI; // CDXUTDialog
		void				*pEditBackground; // CDXUTEditBox
		void				*pDXUTScrollBar;
		D3DCOLOR			clTextColor;
		D3DCOLOR			clInfoColor;
		D3DCOLOR			clDebugColor;
		DWORD				m_lChatWindowBottom;
		struct stChatEntry	chatEntry[100];
		stFontRenderer		*m_pFontRenderer;
		ID3DXSprite			*m_pChatTextSprite;
		ID3DXSprite			*m_pSprite;
		IDirect3DDevice9	*m_pD3DDevice;
		int				m_iRenderMode; // 0 - Direct Mode (slow), 1 - Normal mode
		ID3DXRenderToSurface	*pID3DXRenderToSurface;
		IDirect3DTexture9	*m_pTexture;
		IDirect3DSurface9	*pSurface;
		D3DDISPLAYMODE		*pD3DDisplayMode;
		int					iUnk1[3];
		int					iUnk2; // smth related to drawing in direct mode
		int					m_iRedraw;
		int					m_nPrevScrollBarPosition;
		int					m_iFontSizeY;
		int					m_iTimestampWidth;
	};

	struct stInputBox
	{
		void	*pUnknown;
		uint8_t	bIsChatboxOpen;
		uint8_t	bIsMouseInChatbox;
		uint8_t	bMouseClick_related;
		uint8_t	unk;
		DWORD	dwPosChatInput[2];
		uint8_t	unk2[263];
		int		iCursorPosition;
		uint8_t	unk3;
		int		iMarkedText_startPos; // Highlighted text between this and iCursorPosition
		uint8_t	unk4[20];
		int		iMouseLeftButton;
	};

	typedef void(__cdecl *CMDPROC) (PCHAR);
	struct stInputInfo
	{
		void				*pD3DDevice;
		void				*pDXUTDialog;
		stInputBox			*pDXUTEditBox;
		CMDPROC				pCMDs[SAMP_MAX_CLIENTCMDS];
		char				szCMDNames[SAMP_MAX_CLIENTCMDS][33];
		int					iCMDCount;
		int					iInputEnabled;
		char				szInputBuffer[129];
		char				szRecallBufffer[10][129];
		char				szCurrentBuffer[129];
		int					iCurrentRecall;
		int					iTotalRecalls;
		CMDPROC				pszDefaultCMD;
	};

	struct stKillEntry
	{
		char		szKiller[25];
		char		szVictim[25];
		D3DCOLOR	clKillerColor;
		D3DCOLOR	clVictimColor;
		uint8_t		byteType;
	};

	struct stKillInfo
	{
		int					iEnabled;
		struct stKillEntry	killEntry[5];
		int 			iLongestNickLength;
		int 			iOffsetX;
		int 			iOffsetY;

		ID3DXFont		*pD3DFont;
		ID3DXFont		*pWeaponFont1;
		ID3DXFont		*pWeaponFont2;
		ID3DXSprite		*pSprite;
		IDirect3DDevice9	*pD3DDevice;
		int 			iAuxFontInited;
		ID3DXFont 		*pAuxFont1;
		ID3DXFont 		*pAuxFont2;
	};

	struct stChatPlayer
	{
		int		iCreated;
		char	probablyTheText[256];
		DWORD	dwTickCreated;
		DWORD	dwLiveLength;
		DWORD	dwColor;
		float	fDrawDistance;
		DWORD	dwUnknown;
	};

	struct stAudio {
		int	iSoundState; // 0 - Finished, 1 - Loaded, 2 - Playing
	};

	struct stGameInfo
	{
		struct stCamera
		{
			void*	pEntity; // attached entity
			class CMatrix_Padded* matrix;
		};

		stAudio*	pAudio;
		stCamera*	pCamera;
		stSAMPPed*	pLocalPlayerPed;
		float		fCheckpointPos[3];
		float		fCheckpointExtent[3];
		int			bCheckpointsEnabled;

		// not tested
		DWORD		dwCheckpointMarker;
		float		fRaceCheckpointPos[3];
		float		fRaceCheckpointNext[3];
		float		m_fRaceCheckpointSize;
		uint8_t		byteRaceType;

		int			bRaceCheckpointsEnabled;

		// not tested
		DWORD		dwRaceCheckpointMarker;
		DWORD		dwRaceCheckpointHandle;

		int			iCursorMode;
		uint32_t	ulUnk1;
		int			bClockEnabled;
		uint32_t	ulUnk2;
		int			bHeadMove;
		uint32_t		ulFpsLimit;
		uint8_t		byteUnk3;
		uint8_t		byteVehicleModels[212];
	};

	struct stScoreboardInfo
	{
		int					iIsEnabled;
		int					iPlayersCount;
		float				fTextOffset[2];
		float				fScalar;
		float				fSize[2];
		float				fUnk0[5];
		IDirect3DDevice9	*pDirectDevice;
		class _CDXUTDialog	*pDialog;
		class _CDXUTListBox *pList;
		int					iOffset;		// ?
		int					iIsSorted;		// ?
	};

	struct stActorPool
	{
		int					iLastActorID;
		stSAMPEntity<void>	*pActor[SAMP_MAX_ACTORS]; // ?
		int					iIsListed[SAMP_MAX_ACTORS];
		struct actor_info	*pGTAPed[SAMP_MAX_ACTORS];
		uint32_t			ulUnk0[SAMP_MAX_ACTORS];
		uint32_t			ulUnk1[SAMP_MAX_ACTORS];
	};

	struct stChatBubbleInfo
	{
		struct stChatPlayer	chatBubble[SAMP_MAX_PLAYERS];
	};

	struct stStreamedOutPlayerInfo
	{
		int		iPlayerID[SAMP_MAX_PLAYERS];
		float	fPlayerPos[SAMP_MAX_PLAYERS][3];
	};
#pragma pack(pop)

	//////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////// SUPPORT VARIABLES //////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	struct stTranslateGTASAMP_vehiclePool
	{
		// the index is the position in the vehicle pool
		//
		// translation data
		int iSAMPID[SAMP_MAX_VEHICLES];
	};

	struct stTranslateGTASAMP_pedPool
	{
		// the index is the position in the ped pool
		//
		// translation data
		int iSAMPID[SAMP_MAX_PLAYERS];
	};
	struct stAttachedObject
	{
#pragma pack( 1 )
		int	iModelID;
		int	iBoneID;
		float vecPos[3];
		float vecRot[3];
		float vecScale[3];
		DWORD dwMaterialColor1;
		DWORD dwMaterialColor2;
	};
	struct stDialogResponse {
		WORD wDialogID;
		BYTE bButtonID;
		WORD wListBoxItem;
		unsigned char iInputRespLen;
		char szInputResp[128 + 1];
	};
	struct stClickPlayer {
		uint16_t playerid; //	ид нажатого игрока
		uint8_t  source; //	источник вызова (http://wiki.sa-mp.com/wiki/Click_Sources) 0 - CLICK_SOURCE_SCOREBOARD
	};
#pragma endregion

}

//thanks Yarik
namespace STRUCTSV037R3 {

#pragma region DEFINES
#define SAMP_DIALOG_INFO_OFFSET						0x26E898 //
#define SAMP_CHAT_INFO_OFFSET						0x26E8C8 //
#define SAMP_CHAT_INPUT_INFO_OFFSET					0x26E8CC //
#define SAMP_KILL_INFO_OFFSET						0x26E8D0 //
#define SAMP_INFO_OFFSET							0x26E8DC //
#define SAMP_MISC_INFO								0x26E8F4 //
#define SAMP_SCOREBOARD_INFO						0x26E894 //
#define SAMP_COLOR_OFFSET							0x151578 //
#define SAMP_DRAWING_DISTANCE						0xE60A0 //

#define SAMP_DRAWTEXTLABELS							0x75361 //2
#define SAMP_ONFOOTSENDRATE							0xFE0A8 //
#define SAMP_INCARSENDRATE							0xFE0AC //
#define SAMP_AIMSENDRATE							0xFE0B0 //

#define SAMP_FUNC_ADDTOCHATWND						0x67460 //
#define SAMP_FUNC_SPAWN								0x3AD0 // + not changed&
#define SAMP_FUNC_REQUEST_SPAWN						0x3ED0 //
#define SAMP_FUNC_REQUEST_CLASS						0x56E0 //
#define SAMP_FUNC_DEATH								0x5620 //
#define SAMP_FUNC_SETSPECIALACTION					0x30C0 // + not changed&
#define SAMP_FUNC_SAY								0x5820 //
#define SAMP_FUNC_SENDCMD							0x69190 //
#define SAMP_FUNC_ADDCLIENTCMD						0x69000 //
#define SAMP_FUNC_NAMECHANGE						0xB470 //
#define SAMP_FUNC_RESTARTGAME						0xA1E0 //
#define SAMP_FUNC_ENCRYPT_PORT						0x1CC0 //
#define SAMP_FUNC_TOGGLECURSOR						0x9FFE0 //
#define SAMP_FUNC_CURSORUNLOCKACTORCAM				0x9FEC0 //
#define SAMP_FUNC_UPDATESCOREBOARDDATA				0x8BA0 //
#define SAMP_FUNC_CNETGAMEDESTRUCTOR				0x9510 //
#define SAMP_FUNC_SENDINTERIOR						0x5780 //
#define SAMP_FUNC_DISABLE_SCOREBOARD				0x6E270 //

#define SAMP_HOOKENTER_STATECHANGE					0x15D8B //
#define SAMP_HOOKENTER_HANDLE_RPC					0x3A7ED //
#define SAMP_HOOKENTER_HANDLE_RPC2					0x3A779 //
#define SAMP_HOOKENTER_CNETGAME_DESTR				0xC46E3 //
#define SAMP_HOOKENTER_CNETGAME_DESTR2				0xC5906 //
#define SAMP_HOOKEXIT_ANTICARJACK					0x15D9C //
#define SAMP_HOOKEXIT_HANDLE_RPC					0x3A7F3 //
#define SAMP_HOOKEXIT_HANDLE_RPC2					0x3A801 //

#define SAMP_PATCH_DISABLE_NAMETAGS					0x74C30 //
#define SAMP_PATCH_DISABLE_NAMETAGS_HP				0x73B20 //
#define SAMP_PATCH_SKIPSENDINTERIOR					0x7679 //
#define SAMP_PATCH_SCOREBOARDTOGGLEON				0x6E960 // 
#define SAMP_PATCH_SCOREBOARDTOGGLEONKEYLOCK		0x6EC80 //
#define SAMP_PATCH_CHATINPUTADJUST_Y				0x67436 //
#define SAMP_PATCH_CHATINPUTADJUST_X				0x68D85 //
#pragma endregion
	
#pragma region SAMPSTUFF

	enum DialogStyle
	{
		DIALOG_STYLE_MSGBOX,
		DIALOG_STYLE_INPUT,
		DIALOG_STYLE_LIST,
		DIALOG_STYLE_PASSWORD,
	};

	enum Limits
	{
		SAMP_MAX_ACTORS = 1000,
		SAMP_MAX_PLAYERS = 1004,
		SAMP_MAX_VEHICLES = 2000,
		SAMP_MAX_PICKUPS = 4096,
		SAMP_MAX_OBJECTS = 1000,
		SAMP_MAX_GANGZONES = 1024,
		SAMP_MAX_3DTEXTS = 2048,
		SAMP_MAX_TEXTDRAWS = 2048,
		SAMP_MAX_PLAYERTEXTDRAWS = 256,
		SAMP_MAX_CLIENTCMDS = 144,
		SAMP_MAX_MENUS = 128,
		SAMP_MAX_PLAYER_NAME = 24,
		SAMP_ALLOWED_PLAYER_NAME_LENGTH = 20,
		SAMP_MAX_MAPICONS = 100,
	};

	enum ChatMessageType
	{
		CHAT_TYPE_NONE = 0,
		CHAT_TYPE_CHAT = 2,
		CHAT_TYPE_INFO = 4,
		CHAT_TYPE_DEBUG = 8
	};

	enum ChatDisplayMode
	{
		CHAT_WINDOW_MODE_OFF = 0,
		CHAT_WINDOW_MODE_LIGHT = 1,
		CHAT_WINDOW_MODE_FULL = 2
	};

	enum Gamestate
	{
		GAMESTATE_WAIT_CONNECT = 9,
		GAMESTATE_CONNECTING = 13,
		GAMESTATE_AWAIT_JOIN = 15,
		GAMESTATE_CONNECTED = 14,
		GAMESTATE_RESTARTING = 18
	};

	enum PlayerState
	{
		PLAYER_STATE_NONE = 0,
		PLAYER_STATE_ONFOOT = 17,
		PLAYER_STATE_PASSENGER,
		PLAYER_STATE_DRIVER,
		PLAYER_STATE_WASTED = 32,
		PLAYER_STATE_SPAWNED
	};

	enum MarkersMode
	{
		PLAYER_MARKERS_MODE_OFF,
		PLAYER_MARKERS_MODE_GLOBAL,
		PLAYER_MARKERS_MODE_STREAMED,
	};

	enum SpecialAction
	{
		SPECIAL_ACTION_NONE,
		SPECIAL_ACTION_DUCK,
		SPECIAL_ACTION_USEJETPACK,
		SPECIAL_ACTION_ENTER_VEHICLE,
		SPECIAL_ACTION_EXIT_VEHICLE,
		SPECIAL_ACTION_DANCE1,
		SPECIAL_ACTION_DANCE2,
		SPECIAL_ACTION_DANCE3,
		SPECIAL_ACTION_DANCE4,
		SPECIAL_ACTION_HANDSUP,
		SPECIAL_ACTION_USECELLPHONE,
		SPECIAL_ACTION_SITTING,
		SPECIAL_ACTION_STOPUSECELLPHONE,
		SPECIAL_ACTION_DRINK_BEER = 20,
		SPECIAL_ACTION_SMOKE_CIGGY,
		SPECIAL_ACTION_DRINK_WINE,
		SPECIAL_ACTION_DRINK_SPRUNK,
		SPECIAL_ACTION_CUFFED,
		SPECIAL_ACTION_CARRY,
	};


	//////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////// SAMP STRUCTURES ///////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)
	struct stSAMPPools //R3
	{
		void* pMenu;
		struct stActorPool* pActor;
		struct stPlayerPool* pPlayer;
		struct stVehiclePool* pVehicle;
		struct stPickupPool* pPickup;
		struct stObjectPool* pObject;
		struct stGangzonePool* pGangzone;
		struct stTextLabelPool* pText3D;
		struct stTextdrawPool* pTextdraw;
	};

	struct stSAMP //R3
	{
		uint8_t                  	_pad0[20]; // +20
		void* pUnk0;							// +24
		struct stServerInfo* pServerInfo;		// +28
		uint8_t				_pad1[16];			// + 44
		void* pRakClientInterface;				// +48
		char				szIP[256 + 1];		// +305
		char				szHostname[256 + 1];	// +562
		uint8_t                 	_pad2;			//+563
		bool                    	m_bUpdateCameraTarget;	//+564
		bool				m_bNoNameTagStatus;				//+565
		uint32_t			ulPort;							//+569
		BOOL				m_bLanMode;						//+573
		uint32_t			ulMapIcons[SAMP_MAX_MAPICONS];	// +973
		Gamestate			iGameState;						// +977
		uint32_t			ulConnectTick;					// +981
		struct stServerPresets* pSettings;					// +985
		uint8_t                 	_pad3[5];				//+990
		struct stSAMPPools* pPools;							// +994
	};

	struct stServerInfo {	//R3 Maybe
		uint32_t 			uiIP;
		uint16_t 			usPort;

		// ...

	};

	struct stServerPresets //R3 Maybe
	{
		uint8_t byteCJWalk;
		int m_iDeathDropMoney;
		float	fWorldBoundaries[4];
		bool m_bAllowWeapons;
		float	fGravity;
		uint8_t byteDisableInteriorEnterExits;
		uint32_t ulVehicleFriendlyFire;
		bool m_byteHoldTime;
		bool m_bInstagib;
		bool m_bZoneNames;
		bool m_byteFriendlyFire;
		int		iClassesAvailable;
		float	fNameTagsDistance;
		bool m_bManualVehicleEngineAndLight;
		uint8_t byteWorldTime_Hour;
		uint8_t byteWorldTime_Minute;
		uint8_t byteWeather;
		uint8_t byteNoNametagsBehindWalls;
		int iPlayerMarkersMode;
		float	fGlobalChatRadiusLimit;
		uint8_t byteShowNameTags;
		bool m_bLimitGlobalChatRadius;
	};



	struct stDialogInfo		//R3 maybe
	{
		IDirect3DDevice9* m_pD3DDevice;
		int	iTextPoxX;
		int	iTextPoxY;
		uint32_t	uiDialogSizeX;
		uint32_t	uiDialogSizeY;
		int	iBtnOffsetX;
		int	iBtnOffsetY;
		class _CDXUTDialog* pDialog;
		class _CDXUTListBox* pList;
		class _CDXUTIMEEditBox* pEditBox;
		int	iIsActive;
		int	iType;
		uint32_t	DialogID;
		char* pText;
		uint32_t	uiTextWidth;
		uint32_t	uiTextHeight;
		char		szCaption[65];
		int		bServerside;
		char	pad[536];
	};

	struct stTextDrawTransmit		//R3 maybe
	{
		union
		{
			BYTE byteFlags;
			struct
			{
				BYTE byteBox : 1;
				BYTE byteLeft : 1;
				BYTE byteRight : 1;
				BYTE byteCenter : 1;
				BYTE byteProportional : 1;
				BYTE bytePadding : 3;
			};
		};
		float fLetterWidth;
		float fLetterHeight;
		DWORD dwLetterColor;
		float fBoxWidth;
		float fBoxHeight;
		DWORD dwBoxColor;
		BYTE byteShadow;
		BYTE byteOutline;
		DWORD dwBackgroundColor;
		BYTE byteStyle;
		BYTE byteUNK;
		float fX;
		float fY;
		uint16_t sModel;
		float fRot[3];
		float fZoom;
		WORD sColor[2];
	};

	struct stTextdraw			//R3 maybe
	{
		char		szText[800 + 1];
		char		szString[1600 + 2];
		float		fLetterWidth;
		float		fLetterHeight;
		DWORD		dwLetterColor;
		uint8_t		byte_unk;	// always = 01 (?)
		BYTE		byteCenter;
		BYTE		byteBox;
		float		fBoxSizeX;
		float		fBoxSizeY;
		DWORD		dwBoxColor;
		BYTE		byteProportional;
		DWORD		dwShadowColor;
		BYTE		byteShadowSize;
		BYTE		byteOutline;
		BYTE		byteLeft;
		BYTE		byteRight;
		int			iStyle;		// font style/texture/model
		float		fX;
		float		fY;
		byte		unk[8];
		DWORD		dword99B;	// -1 by default
		DWORD		dword99F;	// -1 by default
		DWORD		index;		// -1 if bad
		BYTE		byte9A7;	// = 1; 0 by default
		uint16_t	sModel;
		float		fRot[3];
		float		fZoom;
		WORD		sColor[2];
		BYTE		f9BE;
		BYTE		byte9BF;
		BYTE		byte9C0;
		DWORD		dword9C1;
		DWORD		dword9C5;
		DWORD		dword9C9;
		DWORD		dword9CD;
		BYTE		byte9D1;
		DWORD		dword9D2;
	};

	struct stTextdrawPool			//R3 maybe
	{
		int					iIsListed[SAMP_MAX_TEXTDRAWS];
		int					iPlayerTextDraw[SAMP_MAX_PLAYERTEXTDRAWS];
		struct stTextdraw* textdraw[SAMP_MAX_TEXTDRAWS];
		struct stTextdraw* playerTextdraw[SAMP_MAX_PLAYERTEXTDRAWS];
	};



	struct stPickup			//R3 maybe
	{
		int		iModelID;
		int		iType;
		float	fPosition[3];
	};

	struct stPickupPool	//R3
	{
		int				iPickupsCount;
		uint32_t		ul_GTA_PickupID[SAMP_MAX_PICKUPS];
		int				iPickupID[SAMP_MAX_PICKUPS];
		int				iTimePickup[SAMP_MAX_PICKUPS];
		uint8_t			unk[SAMP_MAX_PICKUPS * 3];
		/*
		struct stWeaponPickup
		{
		bool  m_bExists;
		short	m_nExOwner;

		};
		*/
		struct stPickup pickup[SAMP_MAX_PICKUPS];
	};

	struct stPlayerPool		//R3 
	{
		uint32_t				ulMaxPlayerID;
		struct stRemotePlayer* pRemotePlayer[SAMP_MAX_PLAYERS];
		int						iIsListed[SAMP_MAX_PLAYERS];
		BOOL					bSavedCheckCollision[SAMP_MAX_PLAYERS];
		int						iLocalPlayerPing;
		int						iLocalPlayerScore;
		uint16_t				sLocalPlayerID;
		void* pVTBL_txtHandler;
		std::string				strLocalPlayerName;
		struct stLocalPlayer* pLocalPlayer;
	};

	/*
	struct stPlayerPool DL
	{
	uint16_t				sLocalPlayerID;
	void* pVTBL_txtHandler;
	std::string				strLocalPlayerName;
	struct stLocalPlayer* pLocalPlayer;
	uint32_t				ulMaxPlayerID;
	struct stRemotePlayer* pRemotePlayer[SAMP_MAX_PLAYERS];
	int					iIsListed[SAMP_MAX_PLAYERS];
	BOOL					bSavedCheckCollision[SAMP_MAX_PLAYERS];
	int					iLocalPlayerPing;
	int					iLocalPlayerScore;
	};
	*/

	struct stSAMPKeys				//unk
	{
		uint8_t keys_primaryFire : 1;
		uint8_t keys_horn__crouch : 1;
		uint8_t keys_secondaryFire__shoot : 1;
		uint8_t keys_accel__zoomOut : 1;
		uint8_t keys_enterExitCar : 1;
		uint8_t keys_decel__jump : 1;			// on foot: jump or zoom in
		uint8_t keys_circleRight : 1;
		uint8_t keys_aim : 1;					// hydra auto aim or on foot aim
		uint8_t keys_circleLeft : 1;
		uint8_t keys_landingGear__lookback : 1;
		uint8_t keys_unknown__walkSlow : 1;
		uint8_t keys_specialCtrlUp : 1;
		uint8_t keys_specialCtrlDown : 1;
		uint8_t keys_specialCtrlLeft : 1;
		uint8_t keys_specialCtrlRight : 1;
		uint8_t keys__unused : 1;
	};

	struct stOnFootData			//R3
	{
		uint16_t	sLeftRightKeys;
		uint16_t	sUpDownKeys;
		union
		{
			uint16_t			sKeys;
			struct stSAMPKeys	stSampKeys;
		};
		float		fPosition[3];
		float		fQuaternion[4];
		uint8_t		byteHealth;
		uint8_t		byteArmor;
		uint8_t		byteCurrentWeapon;
		uint8_t		byteSpecialAction;
		float		fMoveSpeed[3];
		float		fSurfingOffsets[3];
		uint16_t	sSurfingVehicleID;
		short		sCurrentAnimationID;
		short		sAnimFlags;
	};

	struct stInCarData			//R3
	{
		uint16_t	sVehicleID;
		uint16_t	sLeftRightKeys;
		uint16_t	sUpDownKeys;
		union
		{
			uint16_t			sKeys;
			struct stSAMPKeys	stSampKeys;
		};
		float		fQuaternion[4];
		float		fPosition[3];
		float		fMoveSpeed[3];
		float		fVehicleHealth;
		uint8_t		bytePlayerHealth;
		uint8_t		byteArmor;
		uint8_t		byteCurrentWeapon;
		uint8_t		byteSiren;
		uint8_t		byteLandingGearState;
		uint16_t	sTrailerID;
		union
		{
			uint16_t	HydraThrustAngle[2];	//nearly same value
			float		fTrainSpeed;
			float       fBikeSideAngle;
		};
	};

	struct stAimData			//R3
	{
		BYTE	byteCamMode;
		float	vecAimf1[3];
		float	vecAimPos[3];
		float	fAimZ;
		BYTE	byteCamExtZoom : 6;		// 0-63 normalized
		BYTE	byteWeaponState : 2;	// see eWeaponState
		BYTE	byteAspectRatio;
	};

	struct stTrailerData				//R3
	{
		uint16_t	sTrailerID;
		float		fPosition[3];
		float		fQuaternion[4];
		float		fSpeed[3];
		float		fTurnSpeed[3];
	};

	struct stPassengerData				//R3
	{
		uint16_t	sVehicleID;
		uint8_t		byteSeatID;
		uint8_t		byteCurrentWeapon;
		uint8_t		byteHealth;
		uint8_t		byteArmor;
		uint16_t	sLeftRightKeys;
		uint16_t	sUpDownKeys;
		union
		{
			uint16_t			sKeys;
			struct stSAMPKeys	stSampKeys;
		};
		float	fPosition[3];
	};

	struct stDamageData		//R3
	{
		uint16_t	sVehicleID_lastDamageProcessed;
		int			iBumperDamage;
		int			iDoorDamage;
		uint8_t		byteLightDamage;
		uint8_t		byteWheelDamage;
	};

	struct stSurfData //R3
	{
		uint16_t	sSurfingVehicleID;
		uint32_t	ulSurfTick;
		struct stSAMPVehicle* pSurfingVehicle;
		int			m_bStuck;
		int			iIsSurfing;
		float		fSurfPosition[3];
		int			iUnk1;
		int			iSurfMode;	//0 = not surfing, 1 = moving (unstable surf), 2 = fixed on vehicle
	};

	/*
	struct stSurfData //unk
	{
	int			iIsSurfing;
	float		fSurfPosition[3];
	int			iUnk0;
	uint16_t	sSurfingVehicleID;
	uint32_t	ulSurfTick;
	struct stSAMPVehicle* pSurfingVehicle;
	int			iUnk1;
	int			iSurfMode;	//0 = not surfing, 1 = moving (unstable surf), 2 = fixed on vehicle
	};
	*/

	struct stUnoccupiedData //R3
	{
		int16_t sVehicleID;
		uint8_t byteSeatID;
		float	fRoll[3];
		float	fDirection[3];
		float	fPosition[3];
		float	fMoveSpeed[3];
		float	fTurnSpeed[3];
		float	fHealth;
	};

	struct stBulletData	//R3
	{
		uint8_t		byteType;
		uint16_t	sTargetID;
		float		fOrigin[3];
		float		fTarget[3];
		float		fCenter[3];
		uint8_t		byteWeaponID;
	};

	struct stSpectatorData		//R3
	{
		uint16_t	sLeftRightKeys;
		uint16_t	sUpDownKeys;
		union
		{
			uint16_t			sKeys;
			struct stSAMPKeys	stSampKeys;
		};
		float	fPosition[3];
	};

	struct stStatsData	//R3
	{
		int iMoney;
		int iAmmo;	// ?
	};

	struct stHeadSync		//R3
	{
		float	fHeadSync[3];
		int		iHeadSyncUpdateTick;
		int		iHeadSyncLookTick;
	};

	struct CAMERA_TARGET_AIM		//R3
	{
		uint16_t ObjectID;
		uint16_t VehicleID;
		uint16_t PlayerID;
		uint16_t ActorID;
	};

	struct PLAYER_WEAPON_DATA
	{
		uint16_t			sAimingAtPlayerID;
		uint16_t			sAimingAtActorID;
		uint8_t				byteCurrentWeapon;
		uint8_t				byteWeaponInventory[13];
		int				iWeaponAmmo[13];
	};

	struct PLAYER_SPAWN_INFO
	{
		BYTE byteTeam;
		int iSkin;
		//int iCustomModel;
		char unk;
		CVector vecPos;
		float fRotation;
		int iSpawnWeapons[3];
		int iSpawnWeaponsAmmo[3];
	};

	struct stLocalPlayer	//R3
	{
		struct stSAMPPed* pSAMP_Actor;
		struct stInCarData			inCarData;
		struct stAimData			aimData;
		struct stTrailerData		trailerData;
		struct stOnFootData			onFootData;
		struct stPassengerData		passengerData;


		int					iIsActive;
		int					iIsWasted;
		uint16_t			sCurrentVehicleID;
		uint16_t			sLastVehicleID;
		uint16_t			sCurrentAnimID;
		uint16_t			sAnimFlags;
		uint32_t			ulUnk0;
		int					iIsSpectating;
		uint8_t				byteTeamID;
		short				field_10d;
		uint32_t			ulSendTick;
		uint32_t			ulSpectateTick;
		uint32_t			ulAimTick;
		uint32_t			ulStatsUpdateTick;
		CAMERA_TARGET_AIM   cameraTarget;
		uint32_t			ulCameraTargetTick;
		struct stHeadSync	headSyncData;
		uint32_t			ulHeadSyncTick;
		uint32_t			m_lastAnyUpdateTick;
		BOOL				m_bClearedToSpawn;
		uint32_t			ulSpawnSelectionTick;
		uint32_t			ulSpawnSelectionStart;
		PLAYER_SPAWN_INFO   spawnInfo;
		BOOL				m_bHasSpawnInfo;
		uint32_t			ulWeapUpdateTick;
		PLAYER_WEAPON_DATA	m_weaponsData;
		int					iPassengerDriveBy;
		uint8_t				byteCurrentInterior;
		int					iIsInRCVehicle;
		char                szPlayerName[256];
		struct stSurfData	surfData;

		int				iClassSelectionOnDeath;
		int				iSpawnClassID;
		int				iRequestToSpawn;
		int				iIsInSpawnScreen;
		uint32_t			ulDisplayZoneTick;
		uint8_t				byteSpectateMode;		// 3 = vehicle, 4 = player, side = 14, fixed = 15
		uint8_t				byteSpectateType;		// 0 = none, 1 = player, 2 = vehicle
		int				iSpectateID;
		BOOL				iInitiatedSpectating;
		struct stDamageData		vehicleDamageData;
	};

	/*
	struct stLocalPlayer
	{
	struct stSAMPPed* pSAMP_Actor;
	struct stTrailerData		trailerData;
	struct stOnFootData		onFootData;
	struct stPassengerData		passengerData;
	struct stInCarData		inCarData;
	struct stAimData		aimData;
	int				iIsActive;
	int				iIsWasted;
	uint16_t			sCurrentVehicleID;
	uint16_t			sLastVehicleID;
	uint16_t			sCurrentAnimID;
	uint16_t			sAnimFlags;
	uint32_t			ulUnk0;
	CAMERA_TARGET_AIM       	cameraTarget;
	uint32_t			ulCameraTargetTick;
	struct stHeadSync		headSyncData;
	uint32_t			ulHeadSyncTick;
	int				iIsSpectating;
	uint8_t				byteTeamID;
	uint16_t			usUnk2;
	uint32_t			ulSendTick;
	uint32_t			ulSpectateTick;
	uint32_t			ulAimTick;
	uint32_t			ulStatsUpdateTick;
	int				iSpawnClassLoaded;
	uint32_t			ulSpawnSelectionTick;
	uint32_t			ulSpawnSelectionStart;
	PLAYER_SPAWN_INFO      		spawnInfo;
	int				iIsActorAlive;
	uint32_t			ulWeapUpdateTick;
	uint16_t			sAimingAtPlayerID;
	uint16_t			sAimingAtActorID;
	uint8_t				byteCurrentWeapon;
	uint8_t				byteWeaponInventory[13];
	int				iWeaponAmmo[13];
	int				iPassengerDriveBy;
	uint8_t				byteCurrentInterior;
	int				iIsInRCVehicle;
	char                    	szPlayerName[256];
	struct stSurfData		surfData;
	int				iClassSelectionOnDeath;
	int				iSpawnClassID;
	int				iRequestToSpawn;
	int				iIsInSpawnScreen;
	uint32_t			ulDisplayZoneTick;
	uint8_t				byteSpectateMode;		// 3 = vehicle, 4 = player, side = 14, fixed = 15
	uint8_t				byteSpectateType;		// 0 = none, 1 = player, 2 = vehicle
	int				iSpectateID;
	int				iInitiatedSpectating;
	struct stDamageData		vehicleDamageData;
	};
	*/


	struct stRemotePlayerData //R3
	{
		struct stSAMPPed* pSAMP_Actor;				//+4
		struct stSAMPVehicle* pSAMP_Vehicle;		//+8
		uint16_t				sPlayerID;			//+10
		uint16_t				sVehicleID;			//+12
		int						field_1;			//+16
		BOOL					drawlabels;			//+20
		int						iHasJetPack;		//+24
		uint8_t					byteSpecialAction;	//+25

		struct stInCarData		inCarData;
		struct stTrailerData	trailerData;
		struct stAimData		aimData;
		struct stPassengerData	passengerData;
		struct stOnFootData		onFootData;

		uint8_t					byteTeamID;
		uint8_t					bytePlayerState;
		uint8_t					byteSeatID;
		int						field_3;
		BOOL					iPassengerDriveBy;
		float					fOnFootPos[3];
		float					fOnFootMoveSpeed[3];
		float					fVehiclePosition[3];
		float					fVehicleMoveSpeed[3];
		char					pad_1[76];
		CVector					vecVehiclePosOffset;
		float					fVehicleRoll[4];
		float					fActorArmor;
		float					fActorHealth;
		char					pad_2[12];
		int						iLastAnimationID;
		uint8_t					byteUpdateFromNetwork;
		uint32_t				dwTick;
		uint32_t				dwLastStreamedInTick;	// is 0 when currently streamed in
		BOOL                    m_bPerformingCustomAnimation;
		int						iAFKState;			//+469
		struct stHeadSync		headSyncData;		//+473		
		int				iGlobalMarkerLoaded;		//+493
		int				iGlobalMarkerLocation[3];	//+505
		uint32_t			ulGlobalMarker_GTAID;	//+509
	};

	struct stRemotePlayer		//R3
	{
		stRemotePlayerData* pPlayerData;
		int				iPing;
		void* pVTBL_txtHandler;
		std::string			strPlayerName;
		int				iScore;
		int				iIsNPC;




	};

	template <typename T>
	struct stSAMPEntity		//R3
	{
		void* pVTBL;
		uint8_t		byteUnk0[60]; // game CEntity object maybe. always empty.
		T* pGTAEntity;
		uint32_t	ulGTAEntityHandle;
	};

	struct stSAMPPed : public stSAMPEntity < STRUCTSV037R2::actor_info >		//R3 maybe
	{
		int					usingCellPhone;
		uint8_t				byteUnk0[600]; // attached objects info - (state + info + ptr) * 10
		struct actor_info* pGTA_Ped;
		uint8_t				byteUnk1[22];
		uint8_t				byteKeysId;
		uint16_t			ulGTA_UrinateParticle_ID;
		int					DrinkingOrSmoking;
		int					object_in_hand;
		int					drunkLevel;
		uint8_t				byteUnk2[5];
		int					isDancing;
		int					danceStyle;
		int					danceMove;
		uint8_t				byteUnk3[20];
		int					isUrinating;
	};

	struct stVehiclePool		//R3 maybe
	{
		int						iVehicleCount;
		void* pUnk0;
		uint8_t					byteSpace1[0x112C];
		struct stSAMPVehicle* pSAMP_Vehicle[SAMP_MAX_VEHICLES];
		int						iIsListed[SAMP_MAX_VEHICLES];
		struct vehicle_info* pGTA_Vehicle[SAMP_MAX_VEHICLES];
		uint8_t					byteSpace2[SAMP_MAX_VEHICLES * 6];
		uint32_t				ulShit[SAMP_MAX_VEHICLES];
		int						iIsListed2[SAMP_MAX_VEHICLES];
		uint32_t				byteSpace3[SAMP_MAX_VEHICLES * 2];
		float					fSpawnPos[SAMP_MAX_VEHICLES][3];
		int						iInitiated;
	};

	struct stSAMPVehicle : public stSAMPEntity < vehicle_info >	//R3
	{
		struct vehicle_info* m_pTrailer;
		struct vehicle_info* pGTA_Vehicle;
		uint8_t				byteUnk1[8];
		int					bIsMotorOn;
		int					iIsLightsOn;
		int					iIsLocked;
		uint8_t				byteIsObjective;
		int					iObjectiveBlipCreated;
		int					m_timeSinceLastDriven;
		BOOL				m_bHasBeenDriven;
		char				pad_71[4];
		BOOL				m_bEngineState;
		uint8_t				byteColor[2];
		int					iColorSync;
		BOOL						m_bNeedsToUpdateColor;
		BOOL						m_bUnoccupiedSync;
		BOOL						m_bRemoteUnocSync;
		BOOL						m_bKeepModelLoaded;
		int						m_bHasSiren;
		IDirect3DTexture9* m_pLicensePlate;
		char						m_szLicensePlateText[32 + 1]; //MAX_LICENSE_PLATE_TEXT + 1
		uint32_t					m_marker;
	};

	struct stObject : public stSAMPEntity < STRUCTSV037R2::object_info >		//R3 maybe
	{
		uint8_t				byteUnk0[2];
		uint32_t			ulUnk1;
		int					iModel;
		uint16_t			byteUnk2;
		float				fDrawDistance;
		float				fUnk;
		float				fPos[3];
		uint8_t				byteUnk3[68];
		uint8_t				byteUnk4;
		float				fRot[3];
		// ...
	};

	struct stObjectPool			//R3
	{
		int				iObjectCount;
		int				iIsListed[SAMP_MAX_OBJECTS];
		struct stObject* object[SAMP_MAX_OBJECTS];
	};

	struct stGangzone			//R3
	{
		float	fPosition[4];
		DWORD	dwColor;
		DWORD	dwAltColor;
	};

	struct stGangzonePool			//R3
	{
		struct stGangzone* pGangzone[SAMP_MAX_GANGZONES];
		int					iIsListed[SAMP_MAX_GANGZONES];
	};

	struct stTextLabel			//R3
	{
		char* pText;
		DWORD		color;
		float		fPosition[3];
		float		fMaxViewDistance;
		uint8_t		byteShowBehindWalls;
		uint16_t	sAttachedToPlayerID;
		uint16_t	sAttachedToVehicleID;
	};

	struct stTextLabelPool			//R3
	{
		struct stTextLabel	textLabel[SAMP_MAX_3DTEXTS];
		int					iIsListed[SAMP_MAX_3DTEXTS];
	};

	struct stChatEntry			//R3
	{
		uint32_t	SystemTime;
		char		szPrefix[28];
		char		szText[144];
		uint8_t		unknown[64];
		int			iType;			// 2 - text + prefix, 4 - text (server msg), 8 - text (debug)
		D3DCOLOR	clTextColor;
		D3DCOLOR	clPrefixColor;	// or textOnly colour
	};

	struct stFontRenderer		//R3
	{
		ID3DXFont* m_pChatFont;
		ID3DXFont* m_pLittleFont;
		ID3DXFont* m_pChatShadowFont;
		ID3DXFont* m_pLittleShadowFont;
		ID3DXFont* m_pCarNumberFont;
		ID3DXSprite* m_pTempSprite;
		IDirect3DDevice9* m_pD3DDevice;
		char* m_pszTextBuffer;
		int					 m_nCharHeight;
		int					 m_nLittleCharHeight;
	};

	struct stChatInfo			//R3
	{
		int					pagesize;
		char* pLastMsgText;
		int					iChatWindowMode;
		uint8_t				bTimestamps;
		uint32_t			m_iLogFileExist;
		char				logFilePathChatLog[MAX_PATH + 1];
		void* pGameUI; // CDXUTDialog
		void* pEditBackground; // CDXUTEditBox
		void* pDXUTScrollBar;
		D3DCOLOR			clTextColor;
		D3DCOLOR			clInfoColor;
		D3DCOLOR			clDebugColor;
		DWORD				m_lChatWindowBottom;
		struct stChatEntry	chatEntry[100];
		stFontRenderer* m_pFontRenderer;
		ID3DXSprite* m_pChatTextSprite;
		ID3DXSprite* m_pSprite;
		IDirect3DDevice9* m_pD3DDevice;
		int				m_iRenderMode; // 0 - Direct Mode (slow), 1 - Normal mode
		ID3DXRenderToSurface* pID3DXRenderToSurface;
		IDirect3DTexture9* m_pTexture;
		IDirect3DSurface9* pSurface;
		D3DDISPLAYMODE* pD3DDisplayMode;
		int					iUnk1[3];
		int					iUnk2; // smth related to drawing in direct mode
		int					m_iRedraw;
		int					m_nPrevScrollBarPosition;
		int					m_iFontSizeY;
		int					m_iTimestampWidth;
		int					m_iTimeStampTextOffset;
	};

	struct stInputBox		//R3 MayBe
	{
		void* pUnknown;
		uint8_t	bIsChatboxOpen;
		uint8_t	bIsMouseInChatbox;
		uint8_t	bMouseClick_related;
		uint8_t	unk;
		DWORD	dwPosChatInput[2];
		uint8_t	unk2[263];
		int		iCursorPosition;
		uint8_t	unk3;
		int		iMarkedText_startPos; // Highlighted text between this and iCursorPosition
		uint8_t	unk4[20];
		int		iMouseLeftButton;
	};

	typedef void(__cdecl* CMDPROC) (PCHAR);
	struct stInputInfo			//R3
	{
		void* pD3DDevice;
		void* pDXUTDialog;
		stInputBox* pDXUTEditBox;
		CMDPROC				pCMDs[SAMP_MAX_CLIENTCMDS];
		char				szCMDNames[SAMP_MAX_CLIENTCMDS][33];
		int					iCMDCount;
		int					iInputEnabled;
		char				szInputBuffer[129];
		char				szRecallBufffer[10][129];
		char				szCurrentBuffer[129];
		int					iCurrentRecall;
		int					iTotalRecalls;
		CMDPROC				pszDefaultCMD;
	};

	struct stKillEntry		//R3
	{
		char		szKiller[25];
		char		szVictim[25];
		D3DCOLOR	clKillerColor;
		D3DCOLOR	clVictimColor;
		uint8_t		byteType;
	};

	struct stKillInfo			//R3
	{
		int					iEnabled;
		struct stKillEntry	killEntry[5];
		int 			iLongestNickLength;
		int 			iOffsetX;
		int 			iOffsetY;

		ID3DXFont* pD3DFont;
		ID3DXFont* pWeaponFont1;
		ID3DXFont* pWeaponFont2;
		ID3DXSprite* pSprite;
		IDirect3DDevice9* pD3DDevice;
		int 			iAuxFontInited;
		ID3DXFont* pAuxFont1;
		ID3DXFont* pAuxFont2;
	};

	struct stChatPlayer		//unk
	{
		int		iCreated;
		char	probablyTheText[256];
		DWORD	dwTickCreated;
		DWORD	dwLiveLength;
		DWORD	dwColor;
		float	fDrawDistance;
		DWORD	dwUnknown;
	};

	struct stAudio {			//unk
		int	iSoundState; // 0 - Finished, 1 - Loaded, 2 - Playing
		bool bStopInteriourAmbientSounds;
	};

	struct stGameInfo			//R3
	{
		struct stCamera
		{
			void* pEntity; // attached entity
			class CMatrix_Padded* matrix;
		};

		stAudio* pAudio;
		stCamera* pCamera;
		stSAMPPed* pLocalPlayerPed;
		float		fRaceCheckpointPos[3];
		float		fRaceCheckpointNext[3];
		float		m_fRaceCheckpointSize;
		uint8_t		byteRaceType;
		int			bRaceCheckpointsEnabled;
		DWORD		dwRaceCheckpointMarker;
		DWORD		dwRaceCheckpointHandle;
		float		fCheckpointPos[3];
		float		fCheckpointExtent[3];
		int			bCheckpointsEnabled;
		DWORD		dwCheckpointMarker;
		uint32_t	ulUnk2;
		int			bHeadMove;
		uint32_t    ulFpsLimit;
		int			iCursorMode;
		uint32_t	ulUnk1;
		int			bClockEnabled;
		uint8_t		byteUnk3;
		uint8_t		byteVehicleModels[212];
	};

	struct stScoreboardInfo			//R3 
	{
		int					iIsEnabled;
		int					iPlayersCount;
		float				fTextOffset[2];
		float				fScalar;
		float				fSize[2];
		float				fUnk0[5];
		IDirect3DDevice9* pDirectDevice;
		class _CDXUTDialog* pDialog;
		class _CDXUTListBox* pList;
		int					iOffset;		// ?
		int					iIsSorted;		// ?
	};

	struct stActorPool //R3 maybe
	{
		int					iLastActorID;
		stSAMPEntity<void>* pActor[SAMP_MAX_ACTORS]; // ?
		int					iIsListed[SAMP_MAX_ACTORS];
		struct actor_info* pGTAPed[SAMP_MAX_ACTORS];
		uint32_t			ulUnk0[SAMP_MAX_ACTORS];
		uint32_t			ulUnk1[SAMP_MAX_ACTORS];
	};

	struct stChatBubbleInfo		//unk
	{
		struct stChatPlayer	chatBubble[SAMP_MAX_PLAYERS];
	};

	struct stStreamedOutPlayerInfo //unk
	{
		int		iPlayerID[SAMP_MAX_PLAYERS];
		float	fPlayerPos[SAMP_MAX_PLAYERS][3];
	};
#pragma pack(pop)

	//////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////// SUPPORT VARIABLES //////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	struct stTranslateGTASAMP_vehiclePool
	{
		// the index is the position in the vehicle pool
		//
		// translation data
		int iSAMPID[SAMP_MAX_VEHICLES];
	};

	struct stTranslateGTASAMP_pedPool
	{
		// the index is the position in the ped pool
		//
		// translation data
		int iSAMPID[SAMP_MAX_PLAYERS];
	};
	struct stAttachedObject
	{
#pragma pack( 1 )
		int	iModelID;
		int	iBoneID;
		float vecPos[3];
		float vecRot[3];
		float vecScale[3];
		DWORD dwMaterialColor1;
		DWORD dwMaterialColor2;
	};
	struct stDialogResponse {
		WORD wDialogID;
		BYTE bButtonID;
		WORD wListBoxItem;
		char szInputResp[128 + 1];
		unsigned char iInputRespLen;
	};
	struct stClickPlayer {
		uint16_t playerid; //	ид нажатого игрока
		uint8_t  source; //	источник вызова (http://wiki.sa-mp.com/wiki/Click_Sources) 0 - CLICK_SOURCE_SCOREBOARD
	};
#pragma endregion

}