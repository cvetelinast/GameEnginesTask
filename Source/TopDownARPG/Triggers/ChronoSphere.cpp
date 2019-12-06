#include "ChronoSphere.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TopDownARPG.h"
#include "TopDownARPGCharacter.h"
#include "Projectiles/Projectile.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"

AChronoSphere::AChronoSphere()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetGenerateOverlapEvents(true);

	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = SphereComponent;

	OnOverlapParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	OnOverlapParticle->bAutoActivate = true;
}

void AChronoSphere::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTopDownARPG, Display, TEXT("AChronoSphere::BeginOverlap"));
	ActivateParticle();
	ChangeSpeedOfActorOnCollision(Other, SpeedCoefficient);
	StartTimer(Other);
}

void AChronoSphere::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTopDownARPG, Display, TEXT("AChronoSphere::EndOverlap"));
	if (TimerManager == nullptr) {
		UE_LOG(LogTopDownARPG, Error, TEXT("AChronoSphere::OnOverlapEnd TimerManager is nullptr"));
		return;
	}
	if (!TimerHandle.IsValid()) {
		UE_LOG(LogTopDownARPG, Error, TEXT("AChronoSphere::OnOverlapEnd is not valid"));
		return;
	}
	if (!TimerManager->IsTimerActive(TimerHandle)) {
		UE_LOG(LogTopDownARPG, Display, TEXT("AChronoSphere::OnOverlapEnd The Actor already increased its speed on timer end. We shouldn't do this again!"));
		return;
	}

	ChangeSpeedOfActorOnCollision(Other, 1 / SpeedCoefficient);
	StopTimer();
}

void AChronoSphere::ActivateParticle() {
	if (IsValid(OnOverlapParticle))
	{
		OnOverlapParticle->ActivateSystem();
	}
}

void AChronoSphere::ChangeSpeedOfActorOnCollision(AActor* Other, float speedCoeff)
{
	UE_LOG(LogTopDownARPG, Display, TEXT("AChronoSphere::ChangeSpeedOfActorOnCollision current speedCoefficient = %f"), speedCoeff);
	ATopDownARPGCharacter* Character = Cast<ATopDownARPGCharacter>(Other);
	if (IsValid(Character))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed *= speedCoeff;
	}

	AProjectile* Projectile = Cast<AProjectile>(Other);
	if (IsValid(Projectile))
	{
		Projectile->MovementComponent->MaxSpeed *= speedCoeff;
	}
}


void AChronoSphere::StartTimer(AActor* OtherActor) {
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("AChronoSphere::StartTimer IsValid(World) == false"));
		return;
	}
	CustomDelegate.BindUFunction(this, FName("SpeedUpActor"), OtherActor);
	TimerManager = &World->GetTimerManager();
	TimerManager->SetTimer(TimerHandle, CustomDelegate, SlowDownTimeInSeconds, true);
}

void AChronoSphere::StopTimer() {
	if (TimerManager == nullptr) {
		UE_LOG(LogTopDownARPG, Error, TEXT("AChronoSphere::StopTimer TimerManager is nullptr"));
	}
	if (!TimerHandle.IsValid()) {
		UE_LOG(LogTopDownARPG, Error, TEXT("AChronoSphere::TimerHandle is not valid"));
	}
	TimerManager->ClearTimer(TimerHandle);
	UE_LOG(LogTopDownARPG, Display, TEXT("AChronoSphere::StopTimer"));
}

void AChronoSphere::SpeedUpActor(AActor* OtherActor) {
	UE_LOG(LogTopDownARPG, Display, TEXT("AChronoSphere::SpeedUpActor"));
	ChangeSpeedOfActorOnCollision(OtherActor, 1 / SpeedCoefficient);
	StopTimer();
}

void AChronoSphere::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AChronoSphere::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AChronoSphere::OnOverlapEnd);
}

void AChronoSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
