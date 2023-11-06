// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlockVariable")
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(BlueprintReadOnly, Category = "BlockVariable")
	int SnapPointIndex;

	UPROPERTY(BlueprintReadOnly, Category = "BlockVariable")
	int SnapPointIndexLength;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Helper functions")
	TArray<FVector> PreTraceCheck();


	UFUNCTION(BlueprintCallable, Category = "Helper functions")
	void HandleBlock(FHitResult HitResult, uint8 bIsHit, FVector EndLocation);

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
