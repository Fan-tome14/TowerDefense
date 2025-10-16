#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerManager.generated.h"

UCLASS()
class TOWERDEFENSE_API ATowerManager : public AActor
{
    GENERATED_BODY()

public:
    ATowerManager();

protected:
    virtual void BeginPlay() override;

public:
    // 🔹 Blueprints des tourelles par niveau
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Towers")
    TSubclassOf<AActor> TowerLevel1BP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Towers")
    TSubclassOf<AActor> TowerLevel2BP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Towers")
    TSubclassOf<AActor> TowerLevel3BP;

    // 🔹 Positions fixes de spawn des tourelles
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Towers")
    TArray<FVector> TowerSpawnLocations;

    // 🔹 Niveau actuel des tourelles
    UPROPERTY(BlueprintReadOnly, Category = "Towers")
    int32 CurrentTowerLevel;

    // 🔹 Références aux tourelles actuellement présentes
    UPROPERTY()
    TArray<AActor*> SpawnedTowers;

    // 🔹 Fait spawn les tourelles du niveau actuel
    UFUNCTION(BlueprintCallable, Category = "Towers")
    void SpawnTowers();

    // 🔹 Améliore les tourelles si le score le permet
    UFUNCTION(BlueprintCallable, Category = "Towers")
    void UpgradeTowers();
};
