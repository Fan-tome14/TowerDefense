#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TowerDefenseGameState.generated.h"

// Forward declarations
class UDataTable;
class ABaseEnemy;

/**
 * GameState gérant les vagues d'ennemis pour le Tower Defense
 */
UCLASS()
class TOWERDEFENSE_API ATowerDefenseGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    /** Constructeur */
    ATowerDefenseGameState();

    /** Initialisation au début du jeu */
    virtual void BeginPlay() override;

    /** Tick appelé chaque frame */
    virtual void Tick(float DeltaSeconds) override;

    // ---------------------------------------------------
    // 🔹 Wave System Data
    // ---------------------------------------------------

    /** DataTable contenant toutes les vagues */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
    UDataTable* WaveDataTable;

    /** Position de spawn par défaut */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
    FVector SpawnLocation;

    /** Rotation de spawn par défaut */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
    FRotator SpawnRotation;

    /** Liste des positions de spawn possibles pour les ennemis */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
    TArray<FVector> SpawnLocations;

    /** Nombre total d'ennemis à spawn pour la vague actuelle */
    int32 TotalEnemiesThisWave;

    // ---------------------------------------------------
    // 🔹 Timer et état de la vague
    // ---------------------------------------------------

    /** Temps restant avant la prochaine vague */
    UPROPERTY(BlueprintReadOnly, Category="Timer")
    float TempsAvantProchaineVague=0.0f;

    /** Obtenir le temps restant avant la prochaine vague */
    UFUNCTION(BlueprintCallable, Category="Timer")
    float GetTempsAvantProchaineVague() const { return TempsAvantProchaineVague; }

    // ---------------------------------------------------
    // 🔹 Vague / Spawning
    // ---------------------------------------------------

    /** Lance le système de vagues depuis le début */
    void StartWaves();

    /** Démarre la vague suivante */
    void StartNextWave();

    /** Spawn un ennemi selon la vague actuelle */
    void SpawnEnemy();

    /** Décrémente le compteur d'ennemis vivants */
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void DecrementAliveEnemies();

    /** Callback quand un ennemi meurt */
    UFUNCTION()
    void OnEnemyDied(ABaseEnemy* DeadEnemy);

private:
    // ---------------------------------------------------
    // 🔹 Variables internes
    // ---------------------------------------------------

    /** Index de la vague actuelle */
    int32 CurrentWaveIndex;

    /** Compteur d'ennemis spawnés dans la vague actuelle */
    int32 SpawnedEnemies;

    /** Compteur d'ennemis vivants dans la vague actuelle */
    int32 AliveEnemies;

    /** Délai avant la prochaine vague */
    float CurrentNextWaveDelay;

    /** Timer pour le spawn des ennemis */
    FTimerHandle SpawnTimer;

    /** Timer pour la prochaine vague */
    FTimerHandle NextWaveTimer;
};
