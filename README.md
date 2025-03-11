# ABYSS PROTOCOL
- 핵 융합 발전을 연구하던 비밀 연구실에서 방사성 물질이 대량 누출되면서 지상은 황폐화된다. 항공 모함을 거점 삼아 하늘로 피신했지만, 게이트가 열리기 시작했고, 그 너머에서 무수한 몬스터들이 쏟아져 나온다. 인류가 최후의 방어선을 지키기 위해 시행한 작전 Abyss Protocol. 플레이어는 갑판 위로 쏟아져 들어오는 몬스터들을 막아내야 한다.
- 해질녘 시작된 침공은 시간이 흐를 수록 하늘을 붉게 물들여 가며, 게이트의 크기도 커집니다. 더 많은 적들이 몰려오고 상대하기 까다로운 몬스터들이 속속 등장합니다. 한정된 자원과 무기를 활용해 몬스터를 처치하고 상자의 열쇠를 얻어 대피처를 확보해야 합니다. 끝이 보이지 않는 사투 속에서 인류의 희망을 지킬 수 있을지는 오로지 플레이어의 손에 달려 있습니다.

## 캐릭터
### 조작
- WASD : 이동
- Space Bar : 점프
- Ctrl : 앉기
- Left Click : 공격
- 1~6 : 퀵슬롯
- Q : 웨폰 휠
- E : 상자 열기 / 상점 메뉴 열기 등 특정 상황 시 상호작용
- Z : 인벤토리
### 애니메이션
- Motion Matching, Animation Montage, Chooser Table 활용한 상황 별 애니메이션 재생
- 즉시 반응 / 지연 반응으로 구분하여 구현
### 인벤토리
- TreeView 인벤토리 구현 (e.g. 배틀그라운드)
- 드래그 앤 드롭으로 아이템 인벤토리에 추가 / 제거

## 게임 플레이
1. 소환된 몬스터 전부 처치
 - 일반 스테이지 : 일반 몹 10 + 스킬 사용 몹 4
 - 보너스 스테이지 : 일반 몹 30
2. 열쇠 아이템 획득
 - 스테이지에 랜덤으로 스폰되는 상자 오픈 (상자 앞에서 E 키)
 - 상자를 열기 위해서 3초의 대기 시간이 필요
 - 레벨의 모든 상자를 열어도 열쇠를 획득하지 못할 수 있음
3. 조건 해금 후 문에 접근해 다음 스테이지 진입 

## 무기
### 권총 Pistol
 - 장탄 : 무제한
 - 데미지 : 5~10, 연사속도 1초당 1발
 - 탄창 소모 X
 - 파츠: 파츠 1개당 데미지 3, 연사속도 0.1초 감소
### 라이플 Rifle
 - 장탄 : 30발
 - 데미지 : 1발 당 20~25 + 연사(1초당 10발)
 - 파츠: 파츠 1개당 데미지 3, 연사속도 0.05초 감소, 최대탄창 5 증가 
### 샷건 Shotgun
 - 장탄: 12발
 - 데미지 : 총알당 5~10(8발로 쪼개짐) + 느린 연사 속도(2초당 1발)
 - 파츠: 파츠 1개당 총알당 데미지 3, 연사속도 0.1초 감소, 최대탄창 5 증가
### 공통 사항
 - 각 총기 별 사운드 적용(사운드 감쇠 및 동시성 적용) / 탄피 배출 적용
 - 각 총기 별 탄환 적용 해당 탄환이 적이나 지형지물에 부딪쳤을 때, 파티클 시스템과 탄흔 데칼 적용
 - 파츠 랜덤 시스템 적용
   - 구매/획득 시 인벤토리에 무기 이름 없이 파츠로만 표시 / 상점에서도 랜덤 파츠로만 표시 됨
   - 해당 무기를 캐릭터가 장착 했을 때 파츠의 개수만큼 해당 총기에 추가 스탯이 적용됨 (중복적용 가능)

## 적
### 베이직
 - 근접 공격
### 메이지
 - 투사체를 던지는 원거리 공격
 - 주변 몬스터 힐
### 탱크
 - 근접 공격
 - 2초간 데미지 반사
 - 죽을 때 폭발
 - 체력 높음
### 레인저
 - 총을 사용한 원거리 공격
 - 5초간 공격속도 2배
 - 사거리가 김
 - 히트스캔 공격
### 어쌔신
 - 근접 공격
 - 3초간 은신
 - 체력 낮음 / 공격력 높음 / 이동속도 높음
### 적 AI
 - 시각과 청각 활용 (플레이어가 공격 시 MakeNoise로 AI가 인지)
 - Investigating / Patrol / Chase 로 행동 패턴 구분
 - 원거리 : EQS 의 Simple Grid 활용해 공격하기에 좋은 장소로 이동 후 공격
    - 몬스터 위치 기준 100~700 떨어져 있고 플레이어가 보이는 동시에 캐릭터에서 먼 곳으로 이동하여 공격
 - 근거리 : 공격 후 플레이어 위치 주위를 빙글빙글 돌며 회피 시도
    - 타겟이 유효 거리 안에 있으면 EQS 의 OnCircle 로 위치 잡음

## UI
### HUD
 - 좌측 상단 : 체력 / 레벨, 경험치 / 킬 카운트 / 현재 점수 (점수 시스템은 게임 기획 상 미구현하였으나 일단 자리 잡아 둠)
 - 중앙 상단 : 일정 거리 안 겨냥한 적의 이름과 체력 바 표시 / 알림 메세지 표시
 - 우측 상단 : 미니맵에 몬스터, 상점, 문 위치 표시 / 킬 로그 / 미션 정보 표시
 - 좌측 중앙 : 아이템 획득 시 알림 메세지 표시
 - 좌측 하단 : 퀵슬롯 키 안내 및 회복 아이템 개수 표시 (수류탄 미구현)
 - 우측 하단 : 현재 무기 아이콘 / 장탄 수, 소지 탄창 수 / 재장전 키 안내
### 웨폰 휠
 - 무기 5종 (근접 무기, 수류탄 미구현) & 회복약 사용 가능
 - 조작 방법 : Q키를 누른 채 마우스를 호버하고 Q키를 떼면 장착 / 사용
 - 아이템 미소지 시 알림 메세지 표시
### 상점
 - 아이템 구매 / 판매 가능
   - 무기 상점의 경우 현재 구매만 가능... 장착 X
 - 업그레이드 파츠 아이템은 Random Parts 로 표시
   - 구매 메뉴 오픈 시 순서를 섞어 랜덤화
