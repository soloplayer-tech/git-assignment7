#include "DroneController.h"
#include "EnhancedInputSubsystems.h"

ADroneController::ADroneController()
{
    // �����ڿ��� �ʱ�ȭ ����
}

void ADroneController::BeginPlay()
{
    Super::BeginPlay();

    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
            LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                SubSystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }
}