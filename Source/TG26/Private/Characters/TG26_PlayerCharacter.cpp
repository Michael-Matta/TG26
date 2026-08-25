// Copyright © 2026 Teka Games. All Rights Reserved.


#include "TG26/Public/Characters/TG26_PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "AbilitySystem/TG26_AbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTags/TG26_GameplayTagsAbility.h"
#include "GameplayTags/TG26_GameplayTagsInput.h"
#include "Input/TG26_InputComponent.h"
#include "Input/TG26_InputConfig.h"
#include "TG26/TG26.h"


// Sets default values
ATG26_PlayerCharacter::ATG26_PlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	// Camera
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 600.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
	
	// Jump
	JumpMaxCount = 2;
	
}

void ATG26_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	MovementState = EMovementState::Walking;
	TG26_AbilitySystemComponent->AddLooseGameplayTag(TG26_GameplayTags::Ability_Movement_Grounded);
	
	if (IsValid(AnimLayerClass))
	{
		GetMesh()->LinkAnimClassLayers(AnimLayerClass);
	}
}


void ATG26_PlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(InputConfig);
	
	// TODO ???? THis is multiplayer code. Can we ditch it?
	const ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	check(LocalPlayer);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputConfig->DefaultMappingContext, 0);
	
	// Project Settings- Engine Input - Default Input Component needs to be set to the custom class	

	// Set up Action bindings
	if (UTG26_InputComponent* TG26_InputComponent = CastChecked<UTG26_InputComponent>(PlayerInputComponent))
	{
		TG26_InputComponent->BindNativeAction(InputConfig, TG26_GameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
		TG26_InputComponent->BindNativeAction(InputConfig, TG26_GameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
		
		TG26_InputComponent->BindAbilityAction(InputConfig, this, &ThisClass::AbilityInputPressed,&ThisClass::AbilityInputReleased);
		
	}
}

void ATG26_PlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	TG26_AbilitySystemComponent->AddLooseGameplayTag(TG26_GameplayTags::Ability_Movement_Grounded);
	TG26_AbilitySystemComponent->RemoveLooseGameplayTag(TG26_GameplayTags::Ability_Movement_DoubleJump); // Resets Double Jump
}

void ATG26_PlayerCharacter::Move(const FInputActionValue& Value)
{
	// Value is a Struct including many things
	const FVector2D InputValue = Value.Get<FVector2D>();
	
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	
	const FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, InputValue.X);
	
	ControlRotation.Roll = 0.0f;
	AddMovementInput(ControlRotation.Vector(), InputValue.Y);
}


void ATG26_PlayerCharacter::Look(const FInputActionValue& Value)
{
	// Value is a Struct including many things
	const FVector2D InputValue = Value.Get<FVector2D>();
	
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(InputValue.Y);
}

void ATG26_PlayerCharacter::AbilityInputPressed(const FGameplayTag InputTag)
{
	TG26_AbilitySystemComponent->AbilityTagPressed(InputTag);
}


void ATG26_PlayerCharacter::AbilityInputReleased(const FGameplayTag InputTag)
{
	TG26_AbilitySystemComponent->AbilityTagReleased(InputTag);
}