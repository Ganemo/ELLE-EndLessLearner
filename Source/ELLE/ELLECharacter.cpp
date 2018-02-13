// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ELLE.h"
#include "ELLECharacter.h"
#include "StaticGameSettings.h"

AELLECharacter::AELLECharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = false; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	if (!StaticGameSettings::hatBPPath.Equals(""))
	{
		FString str = StaticGameSettings::hatBPPath;
		ConstructorHelpers::FObjectFinder<UStaticMesh> hatmesh(*StaticGameSettings::hatBPPath);

		UStaticMeshComponent* cmpt = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), TEXT("Hat"));
		if (cmpt)
		{
			//cmpt->RegisterComponent();
			cmpt->SetRelativeLocation(FVector(12, 4, 1));
			cmpt->SetRelativeRotation(FRotator(0, 90, -90));
			cmpt->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Head"));
			cmpt->SetStaticMesh(hatmesh.Object);
		}
	}

}

/// <summary>
/// Set's up the input component for the player
/// </summary>
/// <param name="PlayerInputComponent">The input component</param>
void AELLECharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	if (StaticGameSettings::gameType == EGameType::ThirdPerson_Perpendicular)
	{
		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AELLECharacter::DoJump);
		PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	}
	else if (StaticGameSettings::gameType == EGameType::ThirdPerson_OverTheShoulder)
	{
		PlayerInputComponent->BindAction("MoveRight_Action", IE_Pressed, this, &AELLECharacter::MoveRight);
		PlayerInputComponent->BindAction("MoveLeft_Action", IE_Pressed, this, &AELLECharacter::MoveLeft);
	}

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AELLECharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AELLECharacter::TouchStopped);
}

/// <summary>
/// Called When the actor is spawned
/// </summary>
void AELLECharacter::BeginPlay()
{
	Super::BeginPlay();

	laneX = StaticGameSettings::numberOfPuzzleOptions % 2 == 0 ? StaticGameSettings::numberOfPuzzleOptions / 2 : (StaticGameSettings::numberOfPuzzleOptions - 1) / 2;
	APlayerController* pc = Cast<APlayerController>(GetController());

	if (pc)
	{
		pc->bShowMouseCursor = true;
		pc->bEnableClickEvents = true;
		pc->bEnableMouseOverEvents = true;
	}


	SetRunnerColor(FVector(StaticGameSettings::bodyColor.B, StaticGameSettings::bodyColor.G, StaticGameSettings::bodyColor.R));
}

/// <summary>
/// standard tick function called often
/// </summary>
/// <param name="dt"></param>
void AELLECharacter::Tick(float dt)
{
	Super::Tick(dt);

	if(IsRunning)
		AddMovementInput(FVector(1.f, 0.f, 0.f), 1);
}

/// <summary>
/// Movement function to move a character right
/// </summary>
void AELLECharacter::MoveRight()
{
	if (laneX + 1 <= StaticGameSettings::numberOfPuzzleOptions - 1)
	{
		laneX += 1;
		FVector vec = this->GetActorRightVector();
		vec = vec * (400);
		this->SetActorLocation(this->GetActorLocation() + vec);
	}
}
/// <summary>
/// Movement function to move a character left
/// </summary>
void AELLECharacter::MoveLeft()
{
	if (laneX - 1 >= 0)
	{
		laneX -= 1;
		FVector vec = this->GetActorRightVector();
		vec = vec * (-400);
		this->SetActorLocation(this->GetActorLocation() + vec);
	}
}
/// <summary>
/// Make character jump
/// </summary>
void AELLECharacter::DoJump()
{
	if (canJump)
		Jump();
}

/// <summary>
/// Used for touch input (default function)
/// </summary>
/// <param name="FingerIndex"></param>
/// <param name="Location"></param>
void AELLECharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

/// <summary>
/// Used for touch input (default function)
/// </summary>
/// <param name="FingerIndex"></param>
/// <param name="Location"></param>
void AELLECharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

/// <summary>
/// Function called when changing lanes in TPOS
/// </summary>
/// <param name="direction"></param>
void AELLECharacter::MoveLanesEmit_Implementation(int direction)
{
	laneX += direction;
}

/// <summary>
/// Set whether the character is allowed to move
/// </summary>
/// <param name="b"></param>
void AELLECharacter::SetIsRunning(bool b)
{
	IsRunning = b;
}
/// <summary>
/// Set whether the character can jump
/// </summary>
/// <param name="b"></param>
void AELLECharacter::SetCanJump(bool b)
{
	canJump = b;
}
