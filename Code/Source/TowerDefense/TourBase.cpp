#include "TourBase.h"
#include "BaseEnemy.h"
#include "MissileBase.h"
#include "ScoreGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ATourBase::ATourBase()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    MeshTour = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshTour"));
    MeshTour->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshTour->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    MeshTour->SetGenerateOverlapEvents(true);
    MeshTour->SetCollisionObjectType(ECC_WorldDynamic);

    MeshTour->SetNotifyRigidBodyCollision(true);
    MeshTour->SetRenderCustomDepth(true); // pour surbrillance si tu veux

    MeshTour->SetupAttachment(RootComponent);

    PointDeTir = CreateDefaultSubobject<USceneComponent>(TEXT("PointDeTir"));
    PointDeTir->SetupAttachment(MeshTour);

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

    // --- Rotation du composant "arme" vers l'ennemi ---
    if (CibleActuelle && ArmeComponent)
    {
        FVector Origine = ArmeComponent->GetComponentLocation();
        FVector Cible = CibleActuelle->GetActorLocation();

        FVector Direction = (Cible - Origine).GetSafeNormal();
        FRotator RotationCible = Direction.Rotation();

        // 🔧 Retire 90° pour corriger l'orientation du mesh
        RotationCible.Yaw -= 90.f;

        // 🔒 Bloque le pitch (optionnel : garde la rotation horizontale)
        RotationCible.Pitch = 0.f;

        // ✅ Applique directement la rotation (instantané)
        ArmeComponent->SetWorldRotation(RotationCible);
    }
}


void ATourBase::InitialiserDepuisData()
{
    if (!DataTour)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTour non assigné pour %s"), *GetName());
        return;
    }

    // ---- Supprimer l'ancien modèle si présent ----
    if (ModeleActeur)
    {
        ModeleActeur->Destroy();
        ModeleActeur = nullptr;
        ArmeComponent = nullptr;
    }

    // ---- Appliquer les nouvelles données ----
    Degats = DataTour->Degats;
    Portee = DataTour->Portee;
    CadenceTir = DataTour->CadenceTir;
    prix = DataTour->CoutAmelioration;

    // ---- Spawn du nouveau modèle visuel ----
    if (DataTour->Modele)
    {
        ModeleActeur = GetWorld()->SpawnActor<AActor>(DataTour->Modele, GetActorTransform());
        if (ModeleActeur)
        {
            ModeleActeur->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

            // Recherche du composant "arme"
            TArray<UActorComponent*> ComposantsArme = ModeleActeur->GetComponentsByTag(USceneComponent::StaticClass(), FName("arme"));
            if (ComposantsArme.Num() > 0)
            {
                ArmeComponent = Cast<USceneComponent>(ComposantsArme[0]);
                UE_LOG(LogTemp, Log, TEXT("%s : composant 'arme' trouvé dans %s"), *GetName(), *ModeleActeur->GetName());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("%s : aucun composant avec le tag 'arme' trouvé."), *GetName());
            }
        }
    }

    // Redémarre le timer de tir
    GetWorldTimerManager().SetTimer(GestionTir, this, &ATourBase::TirerSurCible, CadenceTir, true);
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

void ATourBase::AmeliorerTour()
{
    if (!DataTour || !DataTour->NiveauSuivant)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s : pas de niveau suivant disponible !"), *GetName());
        return;
    }

    UWorld* World = GetWorld();
    if (!World) return;

    UScoreGameInstance* GameInstance = Cast<UScoreGameInstance>(UGameplayStatics::GetGameInstance(World));
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Impossible de trouver ScoreGameInstance !"));
        return;
    }

    // Vérifie le score (ou ressources)
    if (GameInstance->Score < DataTour->CoutAmelioration)
    {
        UE_LOG(LogTemp, Warning, TEXT("Pas assez de points pour améliorer %s"), *GetName());
        return;
    }

    // Déduit le coût
    GameInstance->Score -= DataTour->CoutAmelioration;

    // Passe au niveau suivant
    DataTour = DataTour->NiveauSuivant;
    NiveauActuel++;

    // Recharge les données et le modèle
    InitialiserDepuisData();

    UE_LOG(LogTemp, Log, TEXT("%s améliorée au niveau %d ! Score restant : %d"), *GetName(), NiveauActuel, GameInstance->Score);
}


void ATourBase::AmeliorerToutesLesToursDuType(TSubclassOf<ATourBase> TypeTour)
{
    UWorld* World = GetWorld();
    if (!World) return;

    TArray<AActor*> Tours;
    UGameplayStatics::GetAllActorsOfClass(World, TypeTour, Tours);

    for (AActor* Acteur : Tours)
    {
        ATourBase* Tour = Cast<ATourBase>(Acteur);
        if (Tour)
        {
            Tour->AmeliorerTour(); // Appel à la fonction existante
        }
    }
}
