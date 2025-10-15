#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TourData.generated.h"

UCLASS(BlueprintType)
class TOWERDEFENSE_API UTourData : public UDataAsset
{
    GENERATED_BODY()

public:

    // Vie de la tourelle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques")
    float Vie = 100.f;

    // Dégâts par missile
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques")
    float Degats = 10.f;

    // Portée de tir
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques")
    float Portee = 800.f;

    // Cadence de tir (en secondes)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques")
    float CadenceTir = 2.f;
};
