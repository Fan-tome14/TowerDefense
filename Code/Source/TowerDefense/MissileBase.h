#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissileBase.generated.h"

class ABaseEnemy; // ✅ correction ici

UCLASS()
class TOWERDEFENSE_API AMissileBase : public AActor
{
    GENERATED_BODY()

public:
    AMissileBase();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
    float Vitesse;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
    float Degats;

    ABaseEnemy* Cible; // ✅ correction ici

public:
    void DefinirCible(ABaseEnemy* NouvelleCible) { Cible = NouvelleCible; }
    void DefinirDegats(float NouveauxDegats) { Degats = NouveauxDegats; }
};
