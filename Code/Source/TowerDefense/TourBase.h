#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TourBase.generated.h"

class ABaseEnemy;      // ✅ correction ici
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques Tourelle")
    int32 TypeTour;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques Tourelle")
    float Vie;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques Tourelle")
    float Portee;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques Tourelle")
    float CadenceTir;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistiques Tourelle")
    float Degats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Tourelle")
    TSubclassOf<AMissileBase> ClasseMissile;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Tourelle")
    USceneComponent* PointDeTir;

    FTimerHandle GestionTir;
    ABaseEnemy* CibleActuelle; // ✅ correction ici

    void TrouverEnnemiLePlusProche();
    void TirerSurCible();

public:
    UFUNCTION(BlueprintCallable, Category = "Tourelle")
    void SubirDegats(float Quantite);

    UFUNCTION(BlueprintCallable, Category = "Tourelle")
    void InitialiserTour(int32 Type);
};
