// Fill out your copyright notice in the Description page of Project Settings.


#include "BBaseItem.h"
#include "BCharacter.h"
#include "Components/SphereComponent.h"
// Sets default values
ABBaseItem::ABBaseItem()
{
    PrimaryActorTick.bCanEverTick = false;

    // 루트 컴포넌트 생성 및 설정
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    // 충돌 컴포넌트 생성 및 설정
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    // 겹침만 감지하는 프로파일 설정
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    // 루트 컴포넌트로 설정
    Collision->SetupAttachment(Scene);

    // 스태틱 메시 컴포넌트 생성 및 설정
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);
    // 메시가 불필요하게 충돌을 막지 않도록 하기 위해, 별도로 NoCollision 등으로 설정할 수 있음.

    // Overlap 이벤트 바인딩
    Collision->OnComponentBeginOverlap.AddDynamic(this, &ABBaseItem::OnItemOverlap);
    Collision->OnComponentEndOverlap.AddDynamic(this, &ABBaseItem::OnItemEndOverlap);

}

// Called when the game starts or when spawned
void ABBaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABBaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Overlap!!!"));
        ActivateItem(OtherActor);

        // 플레이어 캐릭터 확인 후 아이템을 드래그 가능하도록 설정
        ABCharacter* OverlappingCharacter = Cast<ABCharacter>(OtherActor);
        if (OverlappingCharacter)
        {
            OverlappingCharacter->SetDraggingItem(this);
        }
    }
}


void ABBaseItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // 일단은 엔드오버랩에서 뭘 해야될지 정해지지 않아 작성 안해놓음
}

void ABBaseItem::ActivateItem(AActor* Activator)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!")));
}

FName ABBaseItem::GetItemType() const
{
	return ItemType;
}

FName ABBaseItem::GetItemName() const
{
	return ItemName;
}

void ABBaseItem::UseItem(AActor* Activator)
{
    // 공통 함수로 사용하기 위해 제작 추후에 상속 받는 대상안에서 추가 구현
}

void ABBaseItem::DestroyItem()
{
	// AActor에서 제공하는 Destroy() 함수로 객체 제거
	Destroy();
}



