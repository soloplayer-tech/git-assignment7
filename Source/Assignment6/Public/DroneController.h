#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DroneController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ASSIGNMENT6_API ADroneController : public APlayerController
{
    GENERATED_BODY()

public:
    ADroneController();

    // �������Ʈ���� ���� �����ϵ��� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* InputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* RollAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* UpDownAction;

protected:
    virtual void BeginPlay() override;
};