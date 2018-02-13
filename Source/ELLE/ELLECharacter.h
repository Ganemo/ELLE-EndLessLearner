// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Engine.h"
#include "GameFramework/Character.h"
#include "ELLECharacter.generated.h"

UCLASS(config=Game)
class AELLECharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

private:

	bool IsRunning = false;
	bool canJump = true;
	
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float dt) override;

	/** Called for side to side input */
	void MoveRight();
	void MoveLeft();
	void DoJump();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	AELLECharacter();

	UPROPERTY(BlueprintReadWrite)
	int laneX = 0;
	bool isShiftingLanes = false;
	float targetShift;
	float currentShift;

	UPROPERTY(BlueprintReadWrite)
	int laneY = 0;

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = lanes)
	void MoveLanesEmit(int direction);
	void MoveLanesEmit_Implementation(int direction);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Color)
	void SetRunnerColor(FVector color);

	UFUNCTION(BlueprintCallable, Category = Movement)
	void SetIsRunning(bool b);

	UFUNCTION(BlueprintCallable, Category = Movement)
	void SetCanJump(bool b);
};
