#include "TourBase.h"
#include "BaseEnemy.h"
#include "MissileBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ATourBase::ATourBase()
{
    PrimaryActorTick.bCanEverTick = true;

    PointDeTir = CreateDefaultSubobject<USceneComponent>(TEXT("PointDeTir"));
    PointDeTir->SetupAttachment(RootComponent);

    Portee = 800.f;
    CadenceTir = 2.f;
    Degats = 10.f;
}

void ATourBase::BeginPlay()
{
    Super::BeginPlay();
    InitialiserDepuisData();
}

void ATourBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TrouverEnnemiLePlusProche();
}

void ATourBase::InitialiserDepuisData()
{
    if (DataTour)
    {
        Degats = DataTour->Degats;
        Portee = DataTour->Portee;
        CadenceTir = DataTour->CadenceTir;

        GetWorldTimerManager().SetTimer(GestionTir, this, &ATourBase::TirerSurCible, CadenceTir, true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTour non assigné pour %s"), *GetName());
    }
}

void ATourBase::TrouverEnnemiLePlusProche()
{
    TArray<AActor*> Ennemis;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), Ennemis);

    float DistancePlusProche = Portee;
    ABaseEnemy* EnnemiPlusProche = nullptr;

    for (AActor* Acteur : Ennemis)
    {
        ABaseEnemy* Ennemi = Cast<ABaseEnemy>(Acteur);
        if (!Ennemi) continue;

        float Distance = FVector::Dist(Ennemi->GetActorLocation(), GetActorLocation());
        if (Distance < DistancePlusProche)
        {
            DistancePlusProche = Distance;
            EnnemiPlusProche = Ennemi;
        }
    }

    CibleActuelle = EnnemiPlusProche;
}

void ATourBase::TirerSurCible()
{
    if (CibleActuelle && MissileBlueprintSoft.IsValid())
    {
        UClass* MissileClass = MissileBlueprintSoft.LoadSynchronous();
        if (MissileClass)
        {
            FVector PositionTir = PointDeTir->GetComponentLocation();
            FRotator RotationTir = (CibleActuelle->GetActorLocation() - PositionTir).Rotation();

            AMissileBase* Missile = GetWorld()->SpawnActor<AMissileBase>(MissileClass, PositionTir, RotationTir);
            if (Missile)
            {
                Missile->DefinirCible(CibleActuelle);
                Missile->DefinirDegats(Degats);
            }
        }
    }
}


