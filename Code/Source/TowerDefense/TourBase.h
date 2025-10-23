#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TourData.h"
#include "Missilebase.h" //  On inclut la classe AMissile directement
#include "TourBase.generated.h"

class ABaseEnemy;

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
    float Portee;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques")
    float CadenceTir;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques")
    float Degats;

    // --- Combat ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<AMissileBase> MissileClass; // ✅ Classe du missile à instancier

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    USceneComponent* PointDeTir;

    FTimerHandle GestionTir;
    ABaseEnemy* CibleActuelle;

    // --- Composant qui pivote (trouvé via tag "arme") ---
    UPROPERTY()
    USceneComponent* ArmeComponent = nullptr;

    void TrouverEnnemiLePlusProche();
    void TirerSurCible();
    void InitialiserDepuisData();
};
