#include "ScoreGameInstance.h"
#include "UEnemyData.h"
#include "Kismet/GameplayStatics.h"

UScoreGameInstance::UScoreGameInstance()
{
    Score = 0;
}

void UScoreGameInstance::AjouterScore()
{

    // Ajouter 10 points � chaque ennemi mort
    Score += 10;

    UE_LOG(LogTemp, Log, TEXT("Score mis � jour : %d "), Score);
}
