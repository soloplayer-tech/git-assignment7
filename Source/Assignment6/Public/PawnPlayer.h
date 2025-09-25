#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnPlayer.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class ASSIGNMENT6_API APawnPlayer : public APawn
{
	GENERATED_BODY()

public:
	APawnPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void IA_Move(const FInputActionValue& Value);

	UFUNCTION()
	void IA_Look(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* CameraComp;

	FVector2D MoveInput; 
	FVector2D LookInput; 

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MouseSensitivity = 200.f;

	UPROPERTY(BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float Speed = 0.f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* FallbackInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FallbackMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FallbackLookAction;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	float GetSpeed() const { return Speed; }
};
