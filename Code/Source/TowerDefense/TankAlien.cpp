#include "TankAlien.h"
#include "EnnemyAIController.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"

ATankAlien::ATankAlien()
{
	PrimaryActorTick.bCanEverTick = true;

	// Utiliser l'AIController dédié
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnnemyAIController::StaticClass();

	// Laisser le CharacterMovement gérer la rotation
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ATankAlien::BeginPlay()
{
	Super::BeginPlay();

	// ✅ Récupération automatique du premier TargetPoint trouvé dans la scène
	if (!TargetPointActor)
	{
		AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATargetPoint::StaticClass());
		if (FoundActor)
		{
			TargetPointActor = Cast<ATargetPoint>(FoundActor);
			UE_LOG(LogTemp, Warning, TEXT("TargetPoint trouvé : %s"), *TargetPointActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Aucun TargetPoint trouvé dans la scène pour %s"), *GetName());
		}
	}

	// Récupération des stats depuis le Data Asset
	if (EnemyData)
	{
		MovementSpeed = EnemyData->Vitesse;
		Vie = EnemyData->Vie;
		TankAlienClass = EnemyData->Nom.ToString();
		TankAlienClassType = EnemyData->EnemyClass;

		// Appliquer la vitesse au CharacterMovementComponent
		GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TankAlienData non assigné pour %s"), *GetName());
	}

	// ✅ Déplacement vers le TargetPoint via NavMesh (si trouvé)
	if (TargetPointActor)
	{
		AEnnemyAIController* AICont = Cast<AEnnemyAIController>(GetController());
		if (AICont)
		{
			AICont->MoveToActor(TargetPointActor, 5.0f); // tolérance distance
			UE_LOG(LogTemp, Warning, TEXT("%s se déplace vers %s"), *GetName(), *TargetPointActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController non trouvé pour %s"), *GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetPoint non trouvé pour %s"), *GetName());
	}
}

void ATankAlien::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotation visuelle vers la cible
	if (TargetPointActor)
	{
		FVector Direction = (TargetPointActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FRotator NewRotation = UKismetMathLibrary::MakeRotFromX(Direction);
		SetActorRotation(NewRotation);
	}
}
