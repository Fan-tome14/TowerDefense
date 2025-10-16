#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UEnemyData.h"
#include "ScoreGameInstance.generated.h"

// 🔹 Delegate pour notifier le score
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);

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

    // 🔹 Delegate BlueprintAssignable
    UPROPERTY(BlueprintAssignable, Category = "Delegates")
    FOnScoreChanged OnScoreChanged;

    // Ajouter un score
    UFUNCTION(BlueprintCallable, Category = "Score")
    void AjouterScore(int points);

    // Retirer de la vie
    UFUNCTION(BlueprintCallable, Category = "Score")
    void RetirerVie(int32 Montant);


};
