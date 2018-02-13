// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrackPiece.generated.h"

UCLASS(config=Game)
class ELLE_API ATrackPiece : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	ATrackPiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* TrackAttachPoint;
	
};
