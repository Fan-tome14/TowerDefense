#include "ScoreGameInstance.h"
#include "Kismet/GameplayStatics.h"

UScoreGameInstance::UScoreGameInstance()
{
    Score = 0;
    Vie = 100;
}

void UScoreGameInstance::AjouterScore(int points)
{
    Score += points;
    UE_LOG(LogTemp, Log, TEXT("Score mis à jour : %d"), Score);
}

void UScoreGameInstance::RetirerVie(int32 Montant)
{
    Vie -= Montant;
    if (Vie < 0)
        Vie = 0;


    UE_LOG(LogTemp, Warning, TEXT("Vie restante : %d"), Vie);
}

void UScoreGameInstance::ResetGame()
{
    Score = 0;
    Vie = 100;

    UE_LOG(LogTemp, Warning, TEXT("GameInstance réinitialisée : Score=%d Vie=%d"), Score, Vie);
}

void UScoreGameInstance::FinishGame()
{
    UE_LOG(LogTemp, Warning, TEXT("Fin du jeu détectée !"));

    // 🔥 Appelle le délégué (notifie les Blueprints)
    OnGameFinished.Broadcast();
}
