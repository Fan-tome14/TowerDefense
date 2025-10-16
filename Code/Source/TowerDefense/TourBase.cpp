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

    // 🔎 Recherche du composant avec le tag "arme"
    TArray<UActorComponent*> ComposantsArme = GetComponentsByTag(USceneComponent::StaticClass(), FName("arme"));

    // Si aucun composant "arme" trouvé, on cherche "arme2"
    if (ComposantsArme.Num() == 0)
    {
        ComposantsArme = GetComponentsByTag(USceneComponent::StaticClass(), FName("arme2"));
    }

    if (ComposantsArme.Num() > 0)
    {
        ArmeComponent = Cast<USceneComponent>(ComposantsArme[0]);

        if (ArmeComponent->ComponentHasTag(FName("arme2")))
        {
            UE_LOG(LogTemp, Log, TEXT("%s : composant 'arme2' trouvé."), *GetName());
        }
        else
            UE_LOG(LogTemp, Log, TEXT("%s : composant 'arme' trouvé."), *GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s : aucun composant avec le tag 'arme' ou 'arme2' trouvé !"), *GetName());
    }
}



void ATourBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TrouverEnnemiLePlusProche();

    if (CibleActuelle && ArmeComponent)
    {
        FVector Origine = ArmeComponent->GetComponentLocation();
        FVector Cible = CibleActuelle->GetActorLocation();

        FVector Direction = (Cible - Origine).GetSafeNormal();
        FRotator RotationCible = Direction.Rotation();

        // 🔍 Vérifie le tag du composant
        if (!ArmeComponent->ComponentHasTag(FName("arme2")))
        {
            // Applique la correction seulement pour les composants normaux ("arme")
            RotationCible.Yaw -= 90.f;
        }

        // 🔒 Garde la tourelle à plat (évite qu’elle vise vers le haut/bas)
        RotationCible.Pitch = 0.f;

        // ✅ Applique la rotation calculée
        ArmeComponent->SetWorldRotation(RotationCible);
    }
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


