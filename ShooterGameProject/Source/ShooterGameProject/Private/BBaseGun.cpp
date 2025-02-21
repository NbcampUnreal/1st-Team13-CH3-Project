#include "BBaseGun.h"
#include "BCharacter.h"  // 캐릭터 헤더 포함
#include "Kismet/GameplayStatics.h"

// 생성자 정의
ABBaseGun::ABBaseGun()
{
    // 탄약 기본값 설정
    AmmoCount = 30;
    MaxAmmo = 30; // 기본 최대 탄약 설정 (필요 시 블루프린트에서 수정 가능)
}

void ABBaseGun::Reload(AActor* PlayerCharacter)
{
    ABCharacter* BCharacter = Cast<ABCharacter>(PlayerCharacter);
    if (!BCharacter)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("리로드 실패: 올바른 캐릭터가 아님!"));
        return;
    }

    // 인벤토리에서 탄약 확인 (추후 구현)
    int32 AmmoNeeded = MaxAmmo - AmmoCount;
    if (AmmoNeeded > 0)
    {
        AmmoCount = MaxAmmo; // 임시로 탄약을 채우는 방식 (추후 인벤토리에서 가져오도록 수정)
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("재장전 완료!"));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("이미 탄창이 가득 참"));
    }
}

void ABBaseGun::Attack()
{
    UE_LOG(LogTemp, Log, TEXT("총 발사 완료!"));
    // 탄약 감소
    AmmoCount--;
}
