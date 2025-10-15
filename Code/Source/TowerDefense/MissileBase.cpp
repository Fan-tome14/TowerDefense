#include "MissileBase.h"
#include "BaseEnemy.h" 
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
        // Infliger les dégâts
        Cible->SubirDegats(Degats);

        //  Jouer l’effet d’impact
        if (EffetImpact)
        {
            
            FVector Echelle(0.5f);
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EffetImpact, GetActorLocation(), FRotator::ZeroRotator, Echelle, true);

        }

        //  Jouer le son d’impact
        if (SonImpact)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), SonImpact, GetActorLocation());
        }

        // Détruire le missile
        Destroy();
    }

}
