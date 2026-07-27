// Copyright © 2026 Teka Games. All Rights Reserved.


#include "AbilitySystem/TG26_GameplayAbility.h"

#include "AbilitySystem/TG26_AbilitySystemComponent.h"


void UTG26_GameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	if (!ActorInfo)
		return;
	AActor* RawOwner = ActorInfo->OwnerActor.Get();
	if (!IsValid(RawOwner))
		return;
	
	if (bActivateAbilityOnGranted && ActorInfo->AbilitySystemComponent.IsValid())
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
}


UTG26_AbilitySystemComponent* UTG26_GameplayAbility::GetOwnerAbilitySystemComponent()
{
	
	return Cast<UTG26_AbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}


// This will be used to resolve a bug with Block- block tags it is supposed to be resolved in 5.6
// bool UTG26_GameplayAbility::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent,
// 	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
// 	FGameplayTagContainer* OptionalRelevantTags) const
// {
	// Define a common lambda to check for blocked tags
// 	bool bBlocked = false;
// 	auto CheckForBlocked = [&](const FGameplayTagContainer& ContainerA, const FGameplayTagContainer& ContainerB)
// 	{
// 		// Do we not have any tags in common?  Then we're not blocked
// 		if (ContainerA.IsEmpty() || ContainerB.IsEmpty() || !ContainerA.HasAny(ContainerB))
// 		{
// 			return;
// 		}
//  
// 		if (OptionalRelevantTags)
// 		{
// 			// Ensure the global blocking tag is only added once
// 			if (!bBlocked)
// 			{
// 				UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
// 				const FGameplayTag& BlockedTag = AbilitySystemGlobals.ActivateFailTagsBlockedTag;
// 				OptionalRelevantTags->AddTag(BlockedTag);
// 			}
//  
// 			// Now append all the blocking tags
// 			OptionalRelevantTags->AppendMatchingTags(ContainerA, ContainerB);
// 		}
//  
// 		bBlocked = true;
// 	};
//  
// 	// Define a common lambda to check for missing required tags
// 	bool bMissing = false;
// 	auto CheckForRequired = [&](const FGameplayTagContainer& TagsToCheck, const FGameplayTagContainer& RequiredTags)
// 	{
// 		// Do we have no requirements, or have met all requirements?  Then nothing's missing
// 		if (RequiredTags.IsEmpty() || TagsToCheck.HasAll(RequiredTags))
// 		{
// 			return;
// 		}
//  
// 		if (OptionalRelevantTags)
// 		{
// 			// Ensure the global missing tag is only added once
// 			if (!bMissing)
// 			{
// 				UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
// 				const FGameplayTag& MissingTag = AbilitySystemGlobals.ActivateFailTagsMissingTag;
// 				OptionalRelevantTags->AddTag(MissingTag);
// 			}
//  
// 			FGameplayTagContainer MissingTags = RequiredTags; 
// 			MissingTags.RemoveTags(TagsToCheck.GetGameplayTagParents());
// 			OptionalRelevantTags->AppendTags(MissingTags);
// 		}
//  
// 		bMissing = true;
// 	};
//  
// 	// Start by checking all of the blocked tags first (so OptionalRelevantTags will contain blocked tags first)
// 	CheckForBlocked(GetAssetTags(),AbilitySystemComponent.GetBlockedAbilityTags());
// 	CheckForBlocked(AbilitySystemComponent.GetOwnedGameplayTags(), ActivationBlockedTags);
// 	if (SourceTags != nullptr)
// 	{
// 		CheckForBlocked(*SourceTags, SourceBlockedTags);
// 	}
// 	if (TargetTags != nullptr)
// 	{
// 		CheckForBlocked(*TargetTags, TargetBlockedTags);
// 	}
//  
// 	// Now check all required tags
// 	CheckForRequired(AbilitySystemComponent.GetOwnedGameplayTags(), ActivationRequiredTags);
// 	if (SourceTags != nullptr)
// 	{
// 		CheckForRequired(*SourceTags, SourceRequiredTags);
// 	}
// 	if (TargetTags != nullptr)
// 	{
// 		CheckForRequired(*TargetTags, TargetRequiredTags);
// 	}
//  
// 	// We succeeded if there were no blocked tags and no missing required tags	
// 	return !bBlocked && !bMissing;
// // }