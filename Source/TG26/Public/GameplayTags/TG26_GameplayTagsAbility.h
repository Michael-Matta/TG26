// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace TG26_GameplayTags
{
	
	// Movement Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Sprinting)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Grounded)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Airborne)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_DoubleJump)
	
	//Cooldown
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Effect_Cooldown_Attack_Light_Staff)
	
	
	// Generic Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_BlockMontage)
	
	// Item Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Player_Weapon_Staff)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Player_Weapon_Secondary)
	
	// Staff Related Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Player_Equip_Staff)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Player_Attack_Light_Staff)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Player_Attack_Heavy_Staff)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Player_Event_Equip_Staff)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Player_Event_Unequip_Staff)
	
}