#include "TourBase.h"
#include "BaseEnemy.h"
#include "MissileBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


#include "Editor.h"       // pour GEditor
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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

    // 🔎 Recherche du composant avec le tag "arme", puis "arme2", puis "arme3"
    TArray<UActorComponent*> ComposantsArme = GetComponentsByTag(USceneComponent::StaticClass(), FName("arme"));

    if (ComposantsArme.Num() == 0)
    {
        ComposantsArme = GetComponentsByTag(USceneComponent::StaticClass(), FName("arme2"));
    }

    if (ComposantsArme.Num() == 0)
    {
        ComposantsArme = GetComponentsByTag(USceneComponent::StaticClass(), FName("arme3"));
    }

    if (ComposantsArme.Num() > 0)
    {
        ArmeComponent = Cast<USceneComponent>(ComposantsArme[0]);

        if (ArmeComponent->ComponentHasTag(FName("arme2"))){UE_LOG(LogTemp, Log, TEXT("%s : composant 'arme2' trouvé."), *GetName());}
            
        else if (ArmeComponent->ComponentHasTag(FName("arme3"))){UE_LOG(LogTemp, Log, TEXT("%s : composant 'arme3' trouvé."), *GetName());}
            
        else
            UE_LOG(LogTemp, Log, TEXT("%s : composant 'arme' trouvé."), *GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s : aucun composant avec le tag 'arme', 'arme2' ou 'arme3' trouvé !"), *GetName());
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

        // 🔍 Vérifie le tag du composant et applique les corrections
        if (ArmeComponent->ComponentHasTag(FName("arme")))
        {
            RotationCible.Yaw -= 90.f;
        }
        else if (ArmeComponent->ComponentHasTag(FName("arme3")))
        {
            RotationCible.Yaw += 90.f; // 🔄 arme3 tourne de 180°
        }
        // "arme" normal n’a pas de correction

        RotationCible.Pitch = 0.f; // garder la tourelle à plat
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




void ATourBase::DessinerPorteeDebug()
{
    UWorld* Monde = nullptr;

#if WITH_EDITOR
    if (GEditor)
    {
        Monde = GEditor->GetEditorWorldContext().World();
    }
#endif

    if (!Monde)
    {
        UE_LOG(LogTemp, Warning, TEXT("Impossible de récupérer le monde pour le debug !"));
        return;
    }

    FVector Position = GetActorLocation();

    DrawDebugSphere(
        Monde,
        Position,
        Portee,
        32,
        FColor::Green,
        false,
        5.0f,
        0,
        2.0f
    );

    DrawDebugCircle(
        Monde,
        Position,
        Portee,
        64,
        FColor::Green,
        false,
        5.0f,
        0,
        2.f,
        FVector(1, 0, 0),
        FVector(0, 1, 0),
        true
    );

    UE_LOG(LogTemp, Log, TEXT("Debug portée dessinée pour %s"), *GetName());
}
