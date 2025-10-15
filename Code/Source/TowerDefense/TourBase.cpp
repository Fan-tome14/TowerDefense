#include "TourBase.h"
#include "BaseEnemy.h" // ✅ correction ici
#include "MissileBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ATourBase::ATourBase()
{
    PrimaryActorTick.bCanEverTick = true;

    PointDeTir = CreateDefaultSubobject<USceneComponent>(TEXT("PointDeTir"));
    PointDeTir->SetupAttachment(RootComponent);

    TypeTour = 1;
    Vie = 100.f;
    Portee = 800.f;
    CadenceTir = 2.f;
    Degats = 10.f;
}

void ATourBase::BeginPlay()
{
    Super::BeginPlay();

    InitialiserTour(TypeTour);

    GetWorldTimerManager().SetTimer(GestionTir, this, &ATourBase::TirerSurCible, CadenceTir, true);
}

void ATourBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TrouverEnnemiLePlusProche();
}

void ATourBase::InitialiserTour(int32 Type)
{
    TypeTour = Type;

    switch (TypeTour)
    {
    case 1:
        Vie = 100.f;
        Degats = 15.f;
        Portee = 800.f;
        CadenceTir = 2.f;
        break;

    case 2:
        Vie = 150.f;
        Degats = 25.f;
        Portee = 1000.f;
        CadenceTir = 1.5f;
        break;

    case 3:
        Vie = 200.f;
        Degats = 40.f;
        Portee = 1200.f;
        CadenceTir = 1.f;
        break;
    }
}

void ATourBase::TrouverEnnemiLePlusProche()
{
    TArray<AActor*> EnnemisTrouves;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), EnnemisTrouves); // ✅ correction ici

    float DistancePlusProche = Portee;
    ABaseEnemy* EnnemiPlusProche = nullptr; // ✅ correction ici

    for (AActor* Acteur : EnnemisTrouves)
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
    if (CibleActuelle && ClasseMissile)
    {
        FVector PositionTir = PointDeTir->GetComponentLocation();
        FRotator RotationTir = (CibleActuelle->GetActorLocation() - PositionTir).Rotation();

        AMissileBase* Missile = GetWorld()->SpawnActor<AMissileBase>(ClasseMissile, PositionTir, RotationTir);

        if (Missile)
        {
            Missile->DefinirCible(CibleActuelle);
            Missile->DefinirDegats(Degats);
        }
    }
}

void ATourBase::SubirDegats(float Quantite)
{
    Vie -= Quantite;
    if (Vie <= 0)
    {
        Destroy();
    }
}
