// Fill out your copyright notice in the Description page of Project Settings.


#include "TGCharacter.h"
#include "TG26/Jetpack/JetpackComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

// Sets default values
ATGCharacter::ATGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Create the Jetpack Component
	JetpackComponent = CreateDefaultSubobject<UJetpackComponent>(TEXT("JetpackComponent"));

}

// Called when the game starts or when spawned
void ATGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	// if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
	// 	
	// 	// Jumping
	// 	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	// 	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	//
	// 	// Moving
	// 	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPPOnlyDemoTPCharacter::Move);
	// 	EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ACPPOnlyDemoTPCharacter::Look);
	//
	// 	// Looking
	// 	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPPOnlyDemoTPCharacter::Look);
	// }
	// else
	// {
	// 	UE_LOG(LogCPPOnlyDemoTP, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	// }
	
}

