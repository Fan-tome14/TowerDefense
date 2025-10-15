#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TourData.h"
#include "TourBase.generated.h"

class ABaseEnemy;
class AMissileBase;

UCLASS()
class TOWERDEFENSE_API ATourBase : public AActor
{
    GENERATED_BODY()
    
public:	
    ATourBase();

protected:
    virtual void BeginPlay() override;

public:	
    virtual void Tick(float DeltaTime) override;

    // ----------------------
    // Données & Composants
    // ----------------------
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    UTourData* DataTour;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Amélioration")
    int32 NiveauActuel = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tour")
    UStaticMeshComponent* MeshTour;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tour")
    USceneComponent* PointDeTir;

    UPROPERTY()
    USceneComponent* ArmeComponent; 

    UPROPERTY()
    AActor* ModeleActeur; // L’acteur du modèle (spawné depuis DataTour)

    // ----------------------
    // Variables de combat
    // ----------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Portee;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float CadenceTir;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Degats;

    UPROPERTY()
    ABaseEnemy* CibleActuelle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
    TSoftClassPtr<AMissileBase> MissileBlueprintSoft;

    FTimerHandle GestionTir;

    // ----------------------
    // Fonctions
    // ----------------------
    void InitialiserDepuisData();
    void TrouverEnnemiLePlusProche();
    void TirerSurCible();
    void AmeliorerTour();
    
};
