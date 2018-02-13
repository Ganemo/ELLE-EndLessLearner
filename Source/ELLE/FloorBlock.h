// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FloorBlock.generated.h"

UCLASS()
class ELLE_API AFloorBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "FloorBlock Functions")
	void InitBlock();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> Text;

};
