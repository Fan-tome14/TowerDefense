#include "Alien.h"
#include "EnnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/TargetPoint.h"
#include "TimerManager.h"

AAlien::AAlien()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnnemyAIController::StaticClass();

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AAlien::BeginPlay()
{
	Super::BeginPlay();

	// ✅ Trouver un TargetPoint automatiquement
	if (!TargetPointActor)
	{
		AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATargetPoint::StaticClass());
		if (FoundActor)
		{
			TargetPointActor = Cast<ATargetPoint>(FoundActor);
			UE_LOG(LogTemp, Warning, TEXT("%s : TargetPoint trouvé -> %s"), *GetName(), *TargetPointActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : Aucun TargetPoint trouvé"), *GetName());
		}
	}

	// ✅ Charger les données de vitesse
	if (EnemyData)
	{
		MovementSpeed = EnemyData->Vitesse;
		Health = EnemyData->Vie;
		GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	}

	// ✅ Petit délai pour laisser l'AIController se posséder correctement
	GetWorldTimerManager().SetTimerForNextTick(this, &AAlien::StartMovingToTarget);
}

void AAlien::StartMovingToTarget()
{
	AEnnemyAIController* AICont = Cast<AEnnemyAIController>(GetController());
	if (AICont && TargetPointActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : Lancement MoveTo vers %s"), *GetName(), *TargetPointActor->GetName());
		AICont->MoveToActor(TargetPointActor, 5.0f);
	}
	else if (!AICont)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : Aucun AIController trouvé dans StartMovingToTarget()"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : Aucun TargetPoint assigné dans StartMovingToTarget()"), *GetName());
	}
}

void AAlien::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetPointActor)
	{
		FVector Direction = (TargetPointActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FRotator NewRotation = UKismetMathLibrary::MakeRotFromX(Direction);
		SetActorRotation(NewRotation);
	}
}
