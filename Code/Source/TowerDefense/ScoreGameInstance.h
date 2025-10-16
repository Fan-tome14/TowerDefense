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

    // Vie actuelle
    UPROPERTY(BlueprintReadOnly, Category = "Score")
    int32 Vie;

    // Ajouter un score selon le DataAsset de l'ennemi
    UFUNCTION(BlueprintCallable, Category = "Score")
    void AjouterScore();

    // Retirer de la vie
    UFUNCTION(BlueprintCallable, Category = "Score")
    void RetirerVie(int32 Montant);
};
