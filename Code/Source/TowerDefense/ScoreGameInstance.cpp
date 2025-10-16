#include "ScoreGameInstance.h"

UScoreGameInstance::UScoreGameInstance()
{
    Score = 0;
    Vie = 100; // On commence avec 100 points de vie
}

void UScoreGameInstance::AjouterScore()
{
    Score += 10;
    UE_LOG(LogTemp, Log, TEXT("Score mis à jour : %d "), Score);
}

void UScoreGameInstance::RetirerVie(int32 Montant)
{
    Vie -= Montant;
    if (Vie < 0)
        Vie = 0;

    UE_LOG(LogTemp, Warning, TEXT("Vie restante : %d"), Vie);
}
