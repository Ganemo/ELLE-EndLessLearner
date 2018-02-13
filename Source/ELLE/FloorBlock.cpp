// Fill out your copyright notice in the Description page of Project Settings.

#include "ELLE.h"
#include "Engine.h"
#include "FloorBlock.h"



AFloorBlock::AFloorBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

/// <summary>
/// Called when aa block is initialized
/// </summary>
void AFloorBlock::InitBlock_Implementation()
{

}

/// <summary>
/// Called when the game starts or when spawned
/// </summary>
void AFloorBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

/// <summary>
/// Called every frame
/// </summary>
/// <param name="DeltaTime"></param>
void AFloorBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

