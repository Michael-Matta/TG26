// Copyright © 2026 Teka Games. All Rights Reserved.


#include "TG26/Public/Characters/TG26_CharacterBase.h"




ATG26_CharacterBase::ATG26_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	GetMesh()->bReceivesDecals = false;

	
}

