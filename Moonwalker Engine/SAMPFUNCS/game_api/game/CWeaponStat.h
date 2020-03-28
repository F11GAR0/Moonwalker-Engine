/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        sdk/game/CWeaponStat.h
*  PURPOSE:     Weapon Stat Manager interface
*  DEVELOPERS:  Cazomino05 < Cazomino05@gmail.com >
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CWEAPONSTAT_H
#define __CWEAPONSTAT_H

class CWeaponStat
{
public:

        virtual eWeaponType             GetWeaponType                   ( void )=0;
        virtual eWeaponSkill            GetWeaponSkillLevel             ( void )=0;

        virtual void                    SetWeaponType                   ( eWeaponType weaponType )=0;
        virtual void                    SetWeaponSkillLevel             ( eWeaponSkill skillLevel )=0;

        //
        // Interface Sets and Gets
        //

        virtual void                        ToggleFlagBits              ( DWORD flagBits )=0;
        virtual void                        SetFlagBits                 ( DWORD flagBits )=0;
        virtual void                        ClearFlagBits               ( DWORD flagBits )=0;
        // For initialization only
        virtual void                        SetFlags                    ( int iFlags )=0;
        virtual bool                        IsFlagSet                   ( DWORD flag )=0;
        virtual int                         GetFlags                    ( void )=0;

        virtual eWeaponModel                GetModel                    ( void )=0;
        virtual void                        SetModel                    ( int iModel )=0;

        virtual eWeaponModel                GetModel2                   ( void )=0;
        virtual void                        SetModel2                   ( int iModel )=0;

        virtual float                       GetWeaponRange              ( void )=0;
        virtual void                        SetWeaponRange              ( float fRange )=0;

        virtual float                       GetTargetRange              ( void )=0;
        virtual void                        SetTargetRange              ( float fRange )=0;

        virtual CVector *                   GetFireOffset               ( void )=0;
        virtual void                        SetFireOffset               ( CVector * vecFireOffset )=0;

        virtual short                       GetDamagePerHit             ( void )=0;
        virtual void                        SetDamagePerHit             ( short sDamagePerHit )=0;

        virtual float                       GetAccuracy                 ( void )=0;
        virtual void                        SetAccuracy                 ( float fAccuracy )=0;

        virtual short                       GetMaximumClipAmmo          ( void )=0;
        virtual void                        SetMaximumClipAmmo          ( short sAccuracy )=0;

        virtual float                       GetMoveSpeed                ( void )=0;
        virtual void                        SetMoveSpeed                ( float fMoveSpeed )=0;

        // projectile/areaeffect only
        virtual float                       GetFiringSpeed              ( void )=0;
        virtual void                        SetFiringSpeed              ( float fFiringSpeed )=0;

        // area effect only
        virtual float                       GetRadius                   ( void )=0;
        virtual void                        SetRadius                   ( float fRadius )=0;

        virtual float                       GetLifeSpan                 ( void )=0;
        virtual void                        SetLifeSpan                 ( float fLifeSpan )=0;

        virtual float                       GetSpread                   ( void )=0;
        virtual void                        SetSpread                   ( float fSpread )=0;

        virtual float                       GetAnimBreakoutTime         ( void )=0;
        virtual void                        SetAnimBreakoutTime         ( float fBreakoutTime )=0;

        virtual eWeaponSlot                 GetSlot                     ( void )=0;
        virtual void                        SetSlot                     ( eWeaponSlot dwSlot )=0;

        virtual eWeaponSkill                GetSkill                    ( void )=0;
        virtual void                        SetSkill                    ( eWeaponSkill weaponSkill )=0;

        virtual float                       GetRequiredStatLevel        ( void )=0;
        virtual void                        SetRequiredStatLevel        ( float fStatLevel )=0;
        virtual void                        SetRequiredStatLevel        ( int iStatLevel )=0;

        virtual DWORD                       GetAnimGroup                ( void )=0;
        virtual void                        SetAnimGroup                ( DWORD dwAnimGroup )=0;

        virtual eFireType                   GetFireType                 ( void )=0;        
        virtual void                        SetFireType                 ( eFireType type )=0;

        // Floats
        virtual float                       GetWeaponAnimLoopStart      ( void )=0;
        virtual void                        SetWeaponAnimLoopStart      ( float animLoopStart )=0;

        virtual float                       GetWeaponAnimLoopStop       ( void )=0;
        virtual void                        SetWeaponAnimLoopStop       ( float animLoopEnd )=0;

        virtual float                       GetWeaponAnimLoopFireTime   ( void )=0;
        virtual void                        SetWeaponAnimLoopFireTime   ( float animFireTime )=0;

        virtual float                       GetWeaponAnim2LoopStart     ( void )=0;
        virtual void                        SetWeaponAnim2LoopStart     ( float anim2LoopStart )=0;

        virtual float                       GetWeaponAnim2LoopStop      ( void )=0;
        virtual void                        SetWeaponAnim2LoopStop      ( float anim2LoopEnd )=0;

        virtual float                       GetWeaponAnim2LoopFireTime  ( void )=0;
        virtual void                        SetWeaponAnim2LoopFireTime  ( float anim2FireTime )=0;

        virtual float                       GetWeaponAnimBreakoutTime   ( void )=0;
        virtual void                        SetWeaponAnimBreakoutTime   ( float animBreakoutTime )=0;

        virtual float                       GetWeaponSpeed              ( void )=0;
        virtual void                        SetWeaponSpeed              ( float fSpeed )=0;

        virtual float                       GetWeaponRadius             ( void )=0;
        virtual void                        SetWeaponRadius             ( float fRadius )=0;
        
        // Ints
        virtual short                       GetAimOffsetIndex           ( void )=0;    
        virtual void                        SetAimOffsetIndex           ( short sIndex )=0;  

        virtual BYTE                        GetDefaultCombo             ( void )=0;
        virtual void                        SetDefaultCombo             ( BYTE defaultCombo )=0;

        virtual BYTE                        GetCombosAvailable          ( void )=0;
        virtual void                        SetCombosAvailable          ( BYTE nCombosAvailable )=0;


		virtual class CWeaponInfoSAInterface * GetInterface				(void) = 0;
};

#endif