// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"

//USTRUCT(BlueprintType)
//struct FClosestPointStruct
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere)
//	float FloatValue;
//
//	UPROPERTY(EditAnywhere)
//	FVector VectorValue;
//
//	UPROPERTY(EditAnywhere)
//	int32 IntValue;
//};
//

UCLASS()
class LEGOBUILDER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:

	// Camera boom component (to manage the camera's position)
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* CameraBoom;

	// Camera component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsHandleComponent* PhysicsHandleComponent = nullptr;

	UPROPERTY()
	float DeltaSeconds;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float ReachDistance;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float SnapDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlockVariable")
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlockVariable")
	USceneComponent* RotationalHelperComponent;

	UPROPERTY(BlueprintReadOnly, Category = "BlockVariable")
	int SnapPointIndex;

	UPROPERTY(BlueprintReadOnly, Category = "BlockVariable")
	int SnapPointIndexLength;

	UPROPERTY(BlueprintReadOnly, Category = "BlockVariable")
	FRotator RotationOffset;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Helper functions")
	TArray<FVector> PreTraceCheck();

	UFUNCTION(BlueprintCallable, Category = "Helper functions")
	void HandleBlock(FHitResult HitResult, uint8 bIsHit, FVector EndLocation);

	UFUNCTION(BlueprintCallable, Category = "Helper functions")
	void ClosestPointCalculate(TArray<FVector> Points, FVector TestLocation, FTransform HitActorTransform, FVector& ClosestPointResult, int& ClosesPointIdxResult, float& DistanceResult);

	UFUNCTION(BlueprintCallable, Category = "Helper functions")
	FRotator WorldRotationOffset();

	UFUNCTION()
	void PlaceBlock(FHitResult HitResult, uint8 bDidHit);

	UFUNCTION()
	void DroppBlock();

#pragma region Input

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Primary Click Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PClickAction;

	/** Secondary Click Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SClickAction;

	/** RotateLeftRight Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RotateLRAction;

	/** RotateUpDown Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RotateUDAction;

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void PrimaryClick(const FInputActionValue& Value);

	UFUNCTION()
	void SecondaryClick(const FInputActionValue& Value);

	UFUNCTION()
	void RotateLeftRight(const FInputActionValue& Value);
	
	UFUNCTION()
	void RotateUpDown(const FInputActionValue& Value);

#pragma endregion
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
