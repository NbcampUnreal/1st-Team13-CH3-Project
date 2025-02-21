// Fill out your copyright notice in the Description page of Project Settings.


#include "BPistolProjectile.h"

// 생성자
ABPistolProjectile::ABPistolProjectile()
{
    // 피스톨 총알에 맞는 특화 설정
    Speed = 1500.f;  // 속도 증가
}

// BeginPlay
void ABPistolProjectile::BeginPlay()
{
    Super::BeginPlay();
    // 피스톨 총알 시작 시 필요한 설정
}

// Tick
void ABPistolProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}