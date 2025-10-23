#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UEnemyData.h"
#include "BaseEnemy.generated.h"

//  Déclaration du délégué (événement)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeathSignature, ABaseEnemy*, EnnemiMort);

UCLASS()
class TOWERDEFENSE_API ABaseEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    ABaseEnemy();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // --- Statistiques de base ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats Ennemi")
    float Vie;

    // --- DataAsset pour savoir le type d’ennemi ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    UUEnemyData* EnemyData;

    // --- Fonction pour encaisser des dégâts ---
    UFUNCTION(BlueprintCallable, Category = "Combat Ennemi")
    void SubirDegats(float Quantite);

    // 🔥 Événement appelé à la mort de l’ennemi
    UPROPERTY(BlueprintAssignable, Category = "Événements")
    FOnEnemyDeathSignature OnEnemyDeath;
};
