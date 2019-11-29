// Fill out your copyright notice in the Description page of Project Settings.


#include "ChronoSphere.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TopDownARPG.h"
#include "TopDownARPGCharacter.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AChronoSphere::AChronoSphere()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetGenerateOverlapEvents(true);

	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = SphereComponent;

	//// declare overlap events
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AChronoSphere::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AChronoSphere::OnOverlapEnd);

	OnOverlapParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	OnOverlapParticle->bAutoActivate = false;
}

void AChronoSphere::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTopDownARPG, Display, TEXT("AChronoSphere::Overlap"));

	ATopDownARPGCharacter* Character = Cast<ATopDownARPGCharacter>(Other);
	if (IsValid(Character))
	{
		Character->ChangeSpeed(SpeedCoefficient);
	}
	StartTimer(Other);
}

void AChronoSphere::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	UE_LOG(LogTopDownARPG, Display, TEXT("AChronoSphere::EndOverlap"));

	ATopDownARPGCharacter* Character = Cast<ATopDownARPGCharacter>(Other);
	if (IsValid(Character))
	{
		Character->ChangeSpeed(1 / SpeedCoefficient);
	}
}

void AChronoSphere::StartTimer(AActor* OtherActor) {
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("UAbility::Activate IsValid(World) == false"));
		return;
	}
	LastTrappedActor = OtherActor;
	TimerManager = &World->GetTimerManager();
	TimerManager->SetTimer(TimerHandle, this, &AChronoSphere::SpeedUpActor, SlowDownTimeInSeconds, true, SlowDownTimeInSeconds);
}

void AChronoSphere::SpeedUpActor() {

	UE_LOG(LogTopDownARPG, Display, TEXT("AChronoSphere::SpeedUpActor"));

	ATopDownARPGCharacter* Character = Cast<ATopDownARPGCharacter>(LastTrappedActor);
	if (IsValid(Character))
	{
		Character->ChangeSpeed(1 / SpeedCoefficient);
	}
}


// Called when the game starts or when spawned
void AChronoSphere::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AChronoSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
