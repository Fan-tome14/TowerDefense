#include "MissileBase.h"
#include "BaseEnemy.h" // ✅ correction ici
#include "Kismet/GameplayStatics.h"

AMissileBase::AMissileBase()
{
    PrimaryActorTick.bCanEverTick = true;
    Vitesse = 1000.f;
    Degats = 10.f;
}

void AMissileBase::BeginPlay()
{
    Super::BeginPlay();
}

void AMissileBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!IsValid(Cible))
    {
        Destroy();
        return;
    }

    FVector Direction = (Cible->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    SetActorLocation(GetActorLocation() + Direction * Vitesse * DeltaTime);

    if (FVector::Dist(GetActorLocation(), Cible->GetActorLocation()) < 50.f)
    {
        Cible->SubirDegats(Degats); // ✅ assure-toi que SubirDegats existe dans ABaseEnemy
        Destroy();
    }
}
