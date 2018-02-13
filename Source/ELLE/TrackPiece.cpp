// Fill out your copyright notice in the Description page of Project Settings.

#include "ELLE.h"
#include "TrackPiece.h"


// Sets default values
ATrackPiece::ATrackPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));;

	TrackAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("TrackAttachPoint"));
	TrackAttachPoint->SetupAttachment(RootComponent);
}

/// <summary>
/// Called when the game starts or when spawned
/// </summary>
void ATrackPiece::BeginPlay()
{
	Super::BeginPlay();
	
}

/// <summary>
/// Called every frame
/// </summary>
/// <param name="DeltaTime"></param>
void ATrackPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

