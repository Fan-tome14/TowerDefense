#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TourData.h"
#include "TourBase.generated.h"

class ABaseEnemy;
class AMissileBase;

UCLASS()
class TOWERDEFENSE_API ATourBase : public AActor
{
    GENERATED_BODY()

public:
    ATourBase();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // --- Statistiques de la tourelle via DataAsset ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques")
    UTourData* DataTour;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques")
    float Vie;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques")
    float Portee;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques")
    float CadenceTir;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques")
    float Degats;

    // --- Combat ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TSoftClassPtr<AMissileBase> MissileBlueprintSoft; // Permet de glisser un BP missile directement

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    USceneComponent* PointDeTir;

    FTimerHandle GestionTir;
    ABaseEnemy* CibleActuelle;

    void TrouverEnnemiLePlusProche();
    void TirerSurCible();
    void InitialiserDepuisData();

public:
    UFUNCTION(BlueprintCallable, Category = "Tourelle")
    void SubirDegats(float Quantite);
};
