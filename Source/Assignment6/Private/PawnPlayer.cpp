#include "PawnPlayer.h"
#include "PawnPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"

APawnPlayer::APawnPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	RootComponent = CapsuleComp;
	CapsuleComp->InitCapsuleSize(42.f, 96.f);
	CapsuleComp->SetCollisionProfileName(TEXT("Pawn"));
	CapsuleComp->SetSimulatePhysics(false);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CapsuleComp);
	MeshComp->SetRelativeLocation(FVector(0.f, 0.f, -96.f)); 

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true; 

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	FallbackInputMappingContext = nullptr;
	FallbackMoveAction = nullptr;
	FallbackLookAction = nullptr;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	MoveSpeed = 600.f;
	MouseSensitivity = 200.f;
	MoveInput = FVector2D::ZeroVector;
	LookInput = FVector2D::ZeroVector;
	Speed = 0.f;
}

void APawnPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void APawnPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!LookInput.IsNearlyZero() && Controller)
	{
		if (APlayerController* PC = Cast<APlayerController>(Controller))
		{
			FRotator CR = PC->GetControlRotation();
			CR.Yaw += LookInput.X * MouseSensitivity * DeltaTime;
			CR.Pitch = FMath::Clamp(CR.Pitch + LookInput.Y * MouseSensitivity * DeltaTime, -85.f, 85.f);
			PC->SetControlRotation(CR);
		}
	}

	if (!MoveInput.IsNearlyZero() && Controller)
	{
		FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		FVector MoveDirection = (ForwardDir * MoveInput.X) + (RightDir * MoveInput.Y);
		MoveDirection = MoveDirection.GetClampedToMaxSize(1.0f);

		FVector Delta = MoveDirection * MoveSpeed * DeltaTime;

		AddActorWorldOffset(Delta, true);

		Speed = MoveDirection.Size() * MoveSpeed;
	}
	else
	{
		Speed = 0.f;
	}
}

void APawnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	APlayerController* PC = Cast<APlayerController>(GetController());
	UInputMappingContext* MappingToAdd = nullptr;
	UInputAction* BindMove = nullptr;
	UInputAction* BindLook = nullptr;

	if (PC)
	{
		if (APawnPlayerController* MyPC = Cast<APawnPlayerController>(PC))
		{
			MappingToAdd = MyPC->InputMappingContext;
			BindMove = MyPC->MoveAction;
			BindLook = MyPC->LookAction;
		}

		if (!MappingToAdd && FallbackInputMappingContext)
		{
			MappingToAdd = FallbackInputMappingContext;
		}
		if (!BindMove && FallbackMoveAction) BindMove = FallbackMoveAction;
		if (!BindLook && FallbackLookAction) BindLook = FallbackLookAction;

		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (MappingToAdd)
				{
					Subsystem->AddMappingContext(MappingToAdd, 0);
					
				}
			}
		}
	}

	if (BindMove)
	{
		EIC->BindAction(BindMove, ETriggerEvent::Triggered, this, &APawnPlayer::IA_Move);
		EIC->BindAction(BindMove, ETriggerEvent::Completed, this, &APawnPlayer::IA_Move);
	}

	if (BindLook)
	{
		EIC->BindAction(BindLook, ETriggerEvent::Triggered, this, &APawnPlayer::IA_Look);
		EIC->BindAction(BindLook, ETriggerEvent::Completed, this, &APawnPlayer::IA_Look);
	}
}

void APawnPlayer::IA_Move(const FInputActionValue& Value)
{

	const FVector2D In = Value.Get<FVector2D>();
	MoveInput = In;
}

void APawnPlayer::IA_Look(const FInputActionValue& Value)
{
	const FVector2D In = Value.Get<FVector2D>();
	LookInput = In;
}