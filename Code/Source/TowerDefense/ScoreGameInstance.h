#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UEnemyData.h"
#include "ScoreGameInstance.generated.h"

/**
 * GameInstance pour gérer le score global
 */
UCLASS()
class TOWERDEFENSE_API UScoreGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UScoreGameInstance();

    // Score actuel
    UPROPERTY(BlueprintReadOnly, Category = "Score")
    int32 Score;

    // Ajouter un score selon le DataAsset de l'ennemi
    UFUNCTION(BlueprintCallable, Category = "Score")
    void AjouterScore();
};
