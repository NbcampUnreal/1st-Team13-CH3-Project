#include "BBaseGun.h"
#include "BCharacter.h"  // 캐릭터 헤더 포함
#include "Kismet/GameplayStatics.h"

// 생성자 정의
ABBaseGun::ABBaseGun()
{
    // 탄약 기본값 설정
    AmmoCount = 30;
    CurrentAmmo = 30;
    MaxAmmo = 30; // 기본 최대 탄약 설정 (필요 시 블루프린트에서 수정 가능)
    ReservedAmmo = 0;
}


void ABBaseGun::AddAmmo(int32 Amount)
{
    // 예비 탄약에 Amount만큼 추가
    ReservedAmmo += Amount;

    UE_LOG(LogTemp, Log, TEXT("예비 탄약 추가: %d"), ReservedAmmo);
}
void ABBaseGun::Reload()
{
    if (ReservedAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("예비 탄약 부족"));
        return;
    }
    
    int32 NeededAmmo = MaxAmmo - CurrentAmmo;
    if (NeededAmmo <= 0) 
    {
        UE_LOG(LogTemp, Warning, TEXT("재장전 불 필요함"));
        return;
    }
    int32 AmmoToLoad = FMath::Min(NeededAmmo, ReservedAmmo);

    CurrentAmmo += AmmoToLoad;
    ReservedAmmo -= AmmoToLoad;

    UE_LOG(LogTemp, Log, TEXT("리로드 완료: 현재 탄약 %d/%d, 예비 탄약 %d"), CurrentAmmo, MaxAmmo, ReservedAmmo);
}

void ABBaseGun::Attack()
{
    UE_LOG(LogTemp, Log, TEXT("총 발사 완료!"));
    // 탄약 감소
    CurrentAmmo--;
}
