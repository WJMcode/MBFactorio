# MBFactorio 팀 프로젝트
## 🎮 프로젝트 개요
- **장르** : 탑다운 샌드박스 게임
- **컨셉** : Factorio를 참고
- **목표** : 플레이어는 자원을 채굴하고, 이를 조합하여 다양한 아이템을 제작하는 것이 목표 
- **기간** : 2025.04.03 ~ 2025.04.17
- **참여 인원** : 3명(전원 개발 참여) <br></br>
![alt text](README_content/Main.png "Title Text")<br></br>

---

## 🛠️ 기술 스택
- **언어** : C++
- **게임 엔진** : Unreal Engine 5.4
- **버전 관리** : Sourcetree, GitHub
- **IDE** : Visual Studio 2022

---

## ⚡ 빌드 및 실행 방법

1. **필수 환경**
   - Unreal Engine 5.4
   - Visual Studio 2022
   - Windows 10/11 64bit

2. **프로젝트 다운로드**
    ```bash
    git clone https://github.com/WJMcode/MBFactorio.git
    ```

3. **프로젝트 열기**
   - `MBFactorio.uproject` 더블클릭 또는 언리얼 에디터에서 직접 열기

4. **빌드**
   - `Generate Visual Studio Project files` 후, Visual Studio에서 빌드

5. **실행**
   - 언리얼 에디터에서 `Play` 클릭

---

## ✨ 주요 기능 요약

- 자원 채굴 및 아이템 제작 중심의 샌드박스 시스템
- 타일 기반 맵 랜덤 생성
- 컴포넌트 기반 자원 채굴 시스템 (MiningComponent)


---

## 💡 설계 포인트
- 데이터 에셋/컴포넌트 기반 구조로 확장성/재사용성 강화
- 랜덤 맵 생성 및 확률적 타일 배치로 플레이 다양성 확보
- 코드 분리 및 예외처리로 UX와 안정성 향상

---

## 🧠 프로젝트 구조

```text
MBFactorio/
├── Source/
│   └── MBFactorio/
│       ├── Components/   # 채굴 등 각종 게임 컴포넌트 (예: MiningComponent)
│       ├── Tiles/        # 타일 관리, 랜덤 배치 등 타일 관련 코드
│       └── ...           # 기타 게임 모듈 및 코드
├── Content/              # 언리얼 에셋
├── README.md
└── ...
```
---

## 👤 기여한 부분

### 1. TileGridManager 시스템
- **타일 클래스 설계** : 공통 기능/메시를 관리하는 Tile 클래스와, Ground/Resource/Structures 등 다양한 하위 클래스 설계
- **타일 데이터 구조화** : TileDataAsset을 활용하여 타일 정보를 관리
- **타일 랜덤 배치** : TileGridManager에서 플레이어 중심으로 다양한 타입의 타일을 확률적으로, 랜덤 회전/머티리얼로 배치
- **핵심 함수** :  
  - `SpawnTiles` 하나로 다양한 타일의 생성 통합  
  - TileDataAsset을 참조해 타입/머티리얼 자동 할당

### 2. MiningComponent 시스템
- **채굴 시스템 설계** : 플레이어가 광물 타일을 오버랩 + 우클릭으로 채굴 가능
- **진행도 시각화** : 채굴 진행 시 HUD에 실시간 진행도 표시, 완료 시 인벤토리 반영
- **컴포넌트화** : 캐릭터에 독립적으로 부착 가능, 재사용성/유지보수성 강화
- **핵심 함수** :  
  - MiningComponent에서 채굴 로직 분리  
  - 채굴 성공 시 HUD/InventoryComponent 연동

---

## 🔍 구현 시 고민 및 문제 해결

- **복수 오버랩 처리** :  
  - 여러 광물 타일과 동시 오버랩 시, 첫 번째 타일만 인식하던 문제
  - → 오버랩 해제 시 `FindOverlappingTile()` 호출, 주변 타일 재탐색 및 상태 동기화

- **UX 개선** :  
  - 채굴 도중 우클릭 미유지/커서 이탈 등 예외처리 강화  
  - 애니메이션 반복 방지 등 사용자 경험 개선

- **구조 리팩토링** :  
  - 컴포넌트 기반 구조 적용으로 코드 재사용성/유지보수성 강화

---

현재 여기까지 진행함
 
## 🔎 세부 구현 (주요 코드/로직)

### 1. 타일 랜덤 배치 및 타입/머티리얼 자동 할당

TileGridManager 클래스의 SpawnTiles 함수는 플레이어 주변에 다양한 종류의 타일을 랜덤 확률로 배치하고,  
타일의 종류(Ground/Resource/Structures)에 따라 타입, 머티리얼, 크기를 유연하게 할당하는 함수입니다.

  - **Ground 타일 :** 머티리얼만 랜덤 지정
  - **Resource 타일 :** 광물 타입 + 머티리얼 세트 중 하나를 랜덤 선택 후 지정
  - **Structures 타일 :** 미리 지정된 타입/머티리얼/크기로 설정
    <br>
    
  - **SpawnTiles 함수의 대표 로직** :
```cpp
for (int32 X = 0; X < GridWidth; ++X)
  for (int32 Y = 0; Y < GridHeight; ++Y)
    if (생성될 확률 통과)
      NewTile = SpawnActor(...);
      if (GroundTile == NewTile)          { 랜덤 머티리얼 }
      else if (ResourceTile == NewTile)   { 타입+머티리얼 세트 랜덤 적용 }
      else if (StructuresTile == NewTile) { 지정값 적용 }
      else                                { 잘못된 타일 클래스 }
```
  - **상세 코드** :
      <details>
        <summary> TileGridManager 클래스의 SpawnTiles 함수 전체 코드 </summary>
	      
       ```cpp
       /* @param TileClass		생성할 타일 명시 (땅, 광물, 구조물 등)
        * @param SpawnProbability   	타일이 생성될 확률 (0.0 ~ 1.0)
        * @param ZOffset		생성 위치의 Z축 보정값(예 : 광물을 땅 위에 살짝 띄우는 용도)
        * @param InTileSize		타일의 크기 설정
        * @param InRotator		타일에 적용할 회전값
        * @param bUseRandomRotation	랜덤 회전값 사용 여부
        */
       void ATileGridManager::SpawnTiles(TSubclassOf<ATile> TileClass, float SpawnProbability, float ZOffset, float InTileSize, FRotator InRotator, bool bUseRandomRotation)
       {
	       // 캐릭터 가져오기
	       ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	       if (!PlayerCharacter || !TileClass) 
	       {
		       UE_LOG(LogTemp, Warning, TEXT("타일 생성 실패: 캐릭터 또는 TileClass 없음"));
		       return;
	       }
       
	       // 캐릭터 발바닥 높이 계산
	       float HalfHeight = PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	       FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	       float FootZValue = PlayerLocation.Z - HalfHeight;

	       // 생성할 타일의 개수
	       int32 GridWidth = TileDataAsset->GridWidth;
	       int32 GridHeight = TileDataAsset->GridHeight;

	       // 구조물 타일인 경우 하나만 생성
	       if(TileClass->IsChildOf(AStructuresTile::StaticClass()))
	       {
		       GridWidth = 1;
		       GridHeight = 1;
	       }

	       // 타일 그리드 중심이 캐릭터 위치로 가도록 보정
	       FVector Origin = PlayerLocation - FVector((GridWidth - 1) * InTileSize * 0.5f, (GridHeight - 1) * InTileSize * 0.5f, 0.f);

	       for (int32 Y = 0; Y < GridHeight; ++Y)
	       {
		       for (int32 X = 0; X < GridWidth; ++X)
		       {
			
			       // 0.0 ~ 1.0 사이의 난수를 뽑아서 확률적으로 타일을 생성
			       // Resource 타일은 부분적으로 생성하기 위함.
			       if (FMath::FRand() > SpawnProbability) continue;

			       FVector Location = Origin + FVector(X * InTileSize, Y * InTileSize, 0.f);
			       Location.Z = FootZValue + ZOffset;	// 발바닥 높이에 생성

			       // bUseRandomRotation가 ture면 랜덤 회전값 적용
			       // false면 인자로 받은 회전값 사용
			       FRotator Rotation = 
										       // 타일을 0, 90, 180, 270도 중 하나로 회전
				       bUseRandomRotation ? FRotator(0.f, FMath::RandRange(0, 3) * 90, 0.f) : InRotator;

			       ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass, Location, Rotation);
			       if (NewTile)
			       {
				       // 타일 크기 설정
				       NewTile->SetTileScale(InTileSize);

				       // 땅 타일일 경우 머티리얼만 랜덤으로 설정
				       if (AGroundTile* GroundTile = Cast<AGroundTile>(NewTile))
				       {
					       if (TileDataAsset && TileDataAsset->GroundTileMaterials.Num() > 0)
					       {
						       GroundTile->SetRandomTileMaterial(TileDataAsset->GroundTileMaterials);
					       }
				       }
				       // 리소스 타일일 경우 타입 설정 및 랜덤 머티리얼 설정
				       else if (AResourceTile* ResourceTile = Cast<AResourceTile>(NewTile))
				       {
					       if (TileDataAsset && TileDataAsset->ResourceTileTypeAndMaterialSet.Num() > 0)
					       {
						       const TArray<FResourceTypeAndMaterials>& Sets = TileDataAsset->ResourceTileTypeAndMaterialSet;
						       int32 Index = FMath::RandRange(0, Sets.Num() - 1);
						       // Sets에 있는 여러 개의 타입 중 하나를 뽑아, 
						       // 타입과 머티리얼 정보를 SelectedSet에 저장
						       const FResourceTypeAndMaterials& SelectedSet = Sets[Index];

						       // SelectedSet에 담긴 타입 정보로 생성된 타일 타입을 설정
						       ResourceTile->SetResourceType(SelectedSet.ResourceType);
						       // SelectedSet에 담긴 머티리얼 배열을 통해 생성된 타일의 머티리얼을 설정
						       ResourceTile->SetRandomTileMaterial(SelectedSet.Materials);
					       }
				       }
				       // 구조물 타일일 경우 한 가지의 머티리얼로 세팅
				       else if (AStructuresTile* StructuresTile = Cast<AStructuresTile>(NewTile))
				       {
					       if (TileDataAsset && TileDataAsset->StructuresTypeAndMaterial.Material)
					       {
						       // StructuresTypeAndMaterial에 담긴 타입 정보로 생성된 타일 타입을 설정
						       StructuresTile->SetStructuresType(TileDataAsset->StructuresTypeAndMaterial.StructuresType);
						       // StructuresTypeAndMaterial에 담긴 머티리얼을 통해 생성된 타일의 머티리얼을 설정
						       StructuresTile->SetTileMaterial(TileDataAsset->StructuresTypeAndMaterial.Material);
						       // 구조물 타입에 따라 크기를 조정 (예: 화로는 세로로 2칸 차지)
						       StructuresTile->SetStructuresTileScale(StructuresTile->GetStructuresType(), InTileSize);
					       }
				       }
				       else
				       {
					       UE_LOG(LogTemp, Warning, TEXT("잘못된 타일 클래스입니다."));
				       }
			       }
			       else
			       {
				       UE_LOG(LogTemp, Warning, TEXT("타일 스폰 실패: (%d, %d)"), X, Y);
			       }
		       }
	       }
       }
       ```
      </details><br>


  - Projectile이 ***Ground Projectile***로 설정된 Skill 사용 시
<br></br>
***Ground Projectile***은 Player 앞에 땅이 있어야 생성되는 발사체.<br>
Player를 중심( 캐릭터의 배꼽 위치 )을 기준으로 전방에 바닥이 존재한다면<br>
Ground Projectile이 생성되고, 바닥이 없다면 생성되지 않습니다.
<br></br>
![groundproject](https://github.com/user-attachments/assets/36e000cf-694d-49c4-94af-ed1080a55919)

      <details>
        <summary> AGroundProjectile 클래스의 BeginPlay 함수 코드 ( GroundProjectile의 생성 위치를 조정 )</summary>
    
     

    
       ```cpp
       /* Skill 데이터 테이블에서 Projectile 설정이 GroundProjectile로 설정된 Skill을 사용하면 GroundProjectile 객체가 생성됩니다.
        * GroundProjectile은 Player의 중심을 기준으로, Skill 데이터 테이블에서 설정한 Transform값을 포함한 위치에 생성됩니다.
        * GroundProjectile이 생성되면 GroundProjectile 위치 기준, 아래 방향으로 LineTrace를 발사하여 오브젝트를 감지합니다.
        * Collision이 Floor로 설정된 오브젝트가 감지되었다면 감지된 오브젝트 위로 GroundProjectile을 옮깁니다.
        * 만약 Collision이 Floor로 설정된 오브젝트가 아닌, 다른 오브젝트가 감지되거나 아무것도 감지되지 않으면
        * 위쪽 방향으로 LineTrace를 발사하여 오브젝트를 감지합니다.
        * 마찬가지로 Collision이 Floor로 설정된 오브젝트가 감지되었다면 감지된 오브젝트 위로 GroundProjectile을 옮깁니다.
        * 이 경우에도 아무것도 감지되지 않으면 GroundProjectile을 Destroy합니다.                     
        */
	void AGroundProjectile::BeginPlay()
	{
		Super::BeginPlay();
	
		// GroundProjectile의 위치 얻어오기
		FVector GroundProjectileLocation = GetActorLocation();
		// 아래 방향으로 LineTrace를 발사
		FHitResult DownHitResult;
		{
			TArray<AActor*> IgnoreActors; IgnoreActors.Add(GetOwner());
	
			// 해당 Trace는 FloorDetectTraceChannel로 발사되는 Trace이다. 
			// 발사된 해당 Trace는 Collision이 Floor로 설정된 오브젝트를 감지한다.
			// Floor로 설정된 오브젝트에만 GroundProjectile 스킬을 스폰시키는 것이 목적.
			const ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel5);
			const bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(),
				GetActorLocation(), GetActorLocation() + FVector(0, 0, -350), TraceTypeQuery,
				false, IgnoreActors, EDrawDebugTrace::ForDuration, DownHitResult, true);
			// 만약 Hit가 발생했다면 그 위치로 GroundProjectile을 옮김
			if (bHit)
			{
				GroundProjectileLocation.Z = DownHitResult.ImpactPoint.Z;
				SetActorLocation(GroundProjectileLocation);
	
				return;
			}
		}
		// 위쪽 방향으로 LineTrace를 발사
		FHitResult UpHitResult;
		{
			TArray<AActor*> IgnoreActors; IgnoreActors.Add(GetOwner());
	
			const ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel5);
			const bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(),
				GetActorLocation(), GetActorLocation() + FVector(0, 0, 200), TraceTypeQuery,
				false, IgnoreActors, EDrawDebugTrace::ForDuration, UpHitResult, true);
	
			if (bHit)
			{
				GroundProjectileLocation.Z = UpHitResult.ImpactPoint.Z;
				SetActorLocation(GroundProjectileLocation);
				
				return;
			}
		}
		
		// Floor가 감지되지 않으면 GroundProjectile을 그냥 제거한다.
		Destroy();
	}
 	```
      </details>

 
      <details>
        <summary> AGroundProjectile 클래스의 OnBeginOverlap 함수 코드 ( GroundProjectile이 다른 오브젝트와 접촉할 시 호출 )</summary>
    
     

    
       ```cpp
       /* GroundProjectile과 Collision이 Floor로 설정된 오브젝트가 접촉하면 Decal Effect를 생성합니다.
        * 접촉했다면 DetectDamageTarget 함수를 호출합니다.
        * DetectDamageTarget 함수는 BoxTrace를 발사하여 데미지를 적용할 수 있는 오브젝트가 있는지 체크하는 함수입니다.
        * 이후 GroundProjectile을 파괴합니다.
        * 데미지를 적용할 수 있는 오브젝트가 존재한다면, UGameplayStatics의 ApplyDamage 함수를 호출하여 데미지를 적용합니다.
        */
	void AGroundProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
	{
		if (!IsValid(this)) { return; }
		
		FTransform NewTransform = GetActorTransform();
		
		// DecalEffect 출력
		{
			FProjectileTableRow* EffectTableRow = DataTableRowHandle.GetRow<FProjectileTableRow>(TEXT("Effect"));
			FEffectDecalTableRow* DecalEffectTableRow = EffectTableRow->EffectTableRowHandle.GetRow<FEffectDecalTableRow>(TEXT("DecalEffect"));
	
			AEffectWithDecal* DecalEffect = GetWorld()->SpawnActorDeferred<AEffectWithDecal>(DecalEffectTableRow->EffectWithDecalClass,
				FTransform::Identity);
	
			FDataTableRowHandle DecalTableRowHandle = EffectTableRow->EffectTableRowHandle;
	
			DecalEffect->SetData(DecalTableRowHandle);
			NewTransform.SetScale3D(DecalEffectTableRow->OverlapParticleTransform.GetScale3D());
			DecalEffect->FinishSpawning(NewTransform);
			// DecalEffect의 위치를 FinishSpawning 함수를 통해 조정하는 것일 뿐,
			// Play 함수를 호출하면 원점에 무조건 DecalEffect가 출력된다.
			DecalEffect->Play();
		}
	
		ABasicPlayer* OwningCharacter = Cast<ABasicPlayer>(GetOwner());
		check(OwningCharacter);
	
		// Skill Data Table 얻어오기
	  	// 스킬을 관리하는 배열에 접근하여 스킬 시전 애니메이션과
	  	// 플레이어가 재생 중인 스킬 시전 애니메이션과 같은 것을 찾아냄.
		// Notify를 발생시킨 애니메이션과 일치하는 애니메이션이 있는 스킬 배열의 인덱스를 저장하고 그 스킬 배열에 있는 데이터 테이블에 접근하여 반환
		const FSkillTableRow* SkillTableRow = OwningCharacter->GetSkillTableRow();
		ensure(SkillTableRow);
	
		// GroundProjectile이 Floor와 Overlap되면 
		// BoxTrace를 이용해 데미지를 줄 타겟을 감지한다.
		AActor* DetectActor = DetectDamageTarget();
	
		Destroy();
	
		// 타겟이 감지되었다면 데미지 주기
		if(DetectActor)
		{
			UGameplayStatics::ApplyDamage(DetectActor, SkillTableRow->SkillDamage, GetInstigator()->GetController(), this, nullptr);
		}
	}
	```
	</details><br>
      
  - Projectile이 ***Straight Projectile***로 설정된 Skill 사용 시
<br></br>
***Straight Projectile***은 Player를 중심으로 일직선으로 발사되는 Projectile.<br>
Straight Projectile이 날아가는 동안 Projectile 주변에 몬스터가 있는지 감지.<br>
몬스터가 감지되었다면 해당 몬스터쪽으로 날아갑니다.
<br></br>
![stra](https://github.com/user-attachments/assets/fa9d8cb8-2b37-4320-853e-5277b883c955)

      <details>
        <summary> AStraightProjectile 클래스의 BeginPlay 함수와 DetectDamageTarget 함수 코드 ( Straight Projectile 생성 시, 범위 내 몬스터를 감지 ) </summary>
    
     

    
       ```cpp
       /* Straight Projectile이 생성되면 AStraightProjectile 클래스의 BeginPlay 함수가 호출됩니다.
        * BeginPlay 함수는 DetectDamageTarget 함수를 호출하여 반환값을 DetectActor에 저장합니다.
        * DetectDamageTarget 함수는 감지된 몬스터를 반환하는 함수입니다.
        * Straight Projectile의 이동 경로 근처에 Collision이 몬스터로 설정된 오브젝트가 있는지 Box Trace를 통해 감지합니다.
        * 감지되었다면 해당 오브젝트를 가리키는 포인터를 반환하고 DetectDamageTarget 함수를 종료합니다.
        */
	void AStraightProjectile::BeginPlay()
	{
		Super::BeginPlay();
	
		DetectActor = DetectDamageTarget();
	}
	
	AActor* AStraightProjectile::DetectDamageTarget()
	{
		FHitResult DetectResult;
		{
		TArray<AActor*> IgnoreActors; IgnoreActors.Add(GetOwner());
	
			FVector TraceStartLocation = GetActorLocation();  // Trace 시작 위치
			FVector TraceDirection = GetActorForwardVector();  // 예: 전방 벡터 (정확한 방향은 상황에 따라 다를 수 있음)
	
			// 새로운 위치 계산
			FVector TraceEndLocation = TraceStartLocation + (TraceDirection * Distance);
	
			// StraightProjectile의 크기를 얻어와서 
			FVector Origin;
			FVector BoxExtent;
			GetActorBounds(false, Origin, BoxExtent);
			
			// Projectile의 자식으로 붙어있는 파티클 시스템의 크기는 빼준다.
			if (ProjectileMeshEffectComponent)
			{
				// 파티클 시스템의 크기 계산
				FBox ParticleBounds = ProjectileMeshEffectComponent->Bounds.GetBox();
				FVector ParticleExtent = ParticleBounds.GetExtent();
	
				// 파티클 시스템 크기를 반영하지 않으려면 BoxExtent에서 빼기
				BoxExtent -= ParticleExtent;
			}
	
			// 감지 범위 조절
			BoxExtent.Y += 50;
			BoxExtent.Z += 150;
			FVector DetectRange = BoxExtent;
	
		     //해당 Trace는 MonsterDetectTraceChannel로 발사되는 Trace이다. 
			// 발사된 해당 Trace는 Collision이 Monster로 설정된 오브젝트를 감지한다.
			const ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel8);
			const bool bHit = UKismetSystemLibrary::BoxTraceSingle(GetWorld(),
				TraceStartLocation, TraceEndLocation, DetectRange, GetOwner()->GetActorRotation(), TraceTypeQuery,
				false, IgnoreActors, EDrawDebugTrace::ForDuration, DetectResult, true);	
	
			if (bHit)
			{
				AActor* TraceDetectActor = DetectResult.GetActor();
	
				if (TraceDetectActor)
				{
					return TraceDetectActor;
				}
			}
		}
		return nullptr;
	}
	```
	</details>

 
      <details>
        <summary> AStraightProjectile 클래스의 Tick 함수와 FollowDamageTarget 함수 코드 ( Straight Projectile이, 감지한 몬스터 쪽으로 이동 ) </summary>
    
     

    
       ```cpp
       /* Straight Projectile이 존재하는 동안 AStraightProjectile 클래스의 Tick 함수가 호출됩니다.
        * Tick 함수에서는 멤버 포인터인 DetectActor가 가리키는 오브젝트가 존재한다면, FollowDamageTarget 함수를 호출합니다.
        * FollowDamageTarget 함수는 인자로 받은 오브젝트(몬스터)를 Straight Projectile이 따라갈 수 있도록 하는 함수입니다.
        * FollowDamageTarget 함수가 호출될 때마다 따라가야 하는 오브젝트가 어떤 방향에 존재하는지 계속 체크합니다.
        * 그리고 Straight Projectile을 해당 방향으로 회전시킵니다.
        * Straight Projectile은 생성 시 일정한 방향으로 이동하고 속도(Velocity)도 이미 설정되어 있으므로, 
        * FollowDamageTarget 함수에서 방향만 설정해주어도 발사체가 타겟으로 이동하게 됩니다.
        */
	void AStraightProjectile::Tick(float DeltaTime)
	{
		Super::Tick(DeltaTime);
	
		if (DetectActor)
		{
			FollowDamageTarget(DetectActor);
		}
	}
	
	void AStraightProjectile::FollowDamageTarget(AActor* TargetActor)
	{
		// 감지된 Actor를 따라가는 함수
		// 타겟 방향을 계산.
		FVector DirectionToTarget = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	
		// 발사체를 타겟 방향으로 회전시킴.
		FRotator NewRotation = DirectionToTarget.Rotation();
		SetActorRotation(NewRotation);
	
		ProjectileMovementComponent->Velocity = DirectionToTarget * ProjectileData->InitialSpeed;
	}
	```
	</details><br>

      
  - Player 피격 시, Overlay Material의 Opacity 값을 조정하여 깜빡이는 효과 부여 
<br></br>
![blinkCha](https://github.com/user-attachments/assets/394c9701-0187-46b3-941f-3b93eed8dc8f)

      <details>
        <summary> UCharacterMeshEffect 클래스의 ApplyHitMaterial 함수 코드 ( UCharacterMeshEffect는 Player의 메시에 효과를 주기 위한 클래스 ) </summary>
    
     

    
       ```cpp
       /* Player의 메시는 오버레이 머티리얼을 가지고 있습니다.
        * Player가 피격될 경우 오버레이 머티리얼의 HitOverlayOpacity라는 Parameter의 값을 변경합니다.
        * 기본적으로 HitOverlayOpacity의 값은 0이며, 피격될 경우 0.6으로 값이 설정되어 Player의 메시가 보라색이 됩니다.
        * 이후 HitOverlayOpacity의 값을 다시 0으로, 그리고 0.6으로 설정하는 것을 반복하여 보라색으로 깜빡이는 효과를 줍니다.
        * Player가 사망하거나, 일정 시간이 지나면 HitOverlayOpacity의 값을 0으로 설정함으로써 깜빡임을 멈춥니다.
        */
	void UCharacterMeshEffect::ApplyHitMaterial(const float Duration)
	{
	    if (!OwningPlayer || !TargetMeshComponent)
	    {
	        UE_LOG(LogTemp, Warning, TEXT("OwningPlayer or MeshComponent is nullptr."));
	        return;
	    }
	
	    // 1. Overlay Material을 가져오기
	    OriginalOverlayMaterial = TargetMeshComponent->GetOverlayMaterial();
	
	    if (!OriginalOverlayMaterial)
	    {
	        UE_LOG(LogTemp, Warning, TEXT("OriginalOverlayMaterial is nullptr, 플레이어 Mesh에는 오버레이 머티리얼이 존재해야 합니다."));
	        return;
	    }
	    
	    // 2. Overlay Material을 동적 머티리얼 인스턴스로 변환
	    UMaterialInstanceDynamic* DynOverlayMaterial = UMaterialInstanceDynamic::Create(OriginalOverlayMaterial, this);
	
	    if (DynOverlayMaterial)
	    {
	        DynOverlayMaterial->SetScalarParameterValue("HitOverlayOpacity", 0.6f);
	        TargetMeshComponent->SetOverlayMaterial(DynOverlayMaterial);
	
	        // BlinkTimerHandle가 작동하고 있지 않을 때에만 아래 코드 실행
	        if (!GetWorld()->GetTimerManager().IsTimerActive(BlinkTimerHandle))
	        {
	            // (Duration / x.f)초마다 BlinkMaterial 함수를 호출함
	            GetWorld()->GetTimerManager().SetTimer(BlinkTimerHandle, [this, DynOverlayMaterial]()
	                {
	                    BlinkMaterial(DynOverlayMaterial);
	                }, Duration / 30.f, true);
	        }
	
	        // RestoreTimerHandle가 작동하고 있지 않을 때에만 아래 코드 실행
	        if (!GetWorld()->GetTimerManager().IsTimerActive(RestoreTimerHandle))
	        {
	            // OwningPlayer가 사망 상태라면 덜 깜빡임
	            if (OwningPlayer && OwningPlayer->GetStatusComponent()->IsDie())
	            {
	                // Duration / x초 후에 타이머를 멈추도록 설정
	                GetWorld()->GetTimerManager().SetTimer(RestoreTimerHandle, [this, DynOverlayMaterial]()
	                    {
	                        RestoreOriginalMaterial(DynOverlayMaterial);
	
	                        // 타이머 정지
	                        GetWorld()->GetTimerManager().ClearTimer(BlinkTimerHandle);
	                        GetWorld()->GetTimerManager().ClearTimer(RestoreTimerHandle);
	
	                        InitializeMembers();
	
	                    }, Duration / 3.f , false);
	            }
	            else
	            {
	                // Duration초 후에 타이머를 멈추도록 설정
	                GetWorld()->GetTimerManager().SetTimer(RestoreTimerHandle, [this, DynOverlayMaterial]()
	                    {
	                        RestoreOriginalMaterial(DynOverlayMaterial);
	
	                        // 타이머 정지
	                        GetWorld()->GetTimerManager().ClearTimer(BlinkTimerHandle);
	                        GetWorld()->GetTimerManager().ClearTimer(RestoreTimerHandle);
	
	                        InitializeMembers();
	
	                    }, Duration, false);
	            }
	        }
	    }
	}
	
	void UCharacterMeshEffect::BlinkMaterial(UMaterialInstanceDynamic* OutDynOverlayMaterial)
	{
	    if (BlinkCount % 2 == 0)
	    {
	        // HitOverlayOpacity값을 0.0f로 설정, 기본 상태랑 같음
	        OutDynOverlayMaterial->SetScalarParameterValue("HitOverlayOpacity", 0.0f);
	    }
	    else
	    {
	        // HitOverlayOpacity값을 0.6f로 설정, OverlayMaterial이 캐릭터에게 반투명 형태로 나타남
	        OutDynOverlayMaterial->SetScalarParameterValue("HitOverlayOpacity", 0.6f);
	    }
	
	    // 파라미터 변경 후 Overlay Material 재설정
	    TargetMeshComponent->SetOverlayMaterial(OutDynOverlayMaterial);
	
	    // BlinkCount 증가
	    ++BlinkCount;
	}
	
	void UCharacterMeshEffect::RestoreOriginalMaterial(UMaterialInstanceDynamic* OutDynOverlayMaterial)
	{
	    TargetMeshComponent->SetOverlayMaterial(OriginalOverlayMaterial);
	}
	
	void UCharacterMeshEffect::InitializeMembers()
	{
	
	    OriginalOverlayMaterial = nullptr;
	
	    BlinkCount = 0;
	    BlinkTimerHandle.Invalidate();
	    RestoreTimerHandle.Invalidate(); 
	}
	```
	</details><br>
                    



### 👾 Monster
  - Monster 사망 시, Material을 교체하고 Opacity 값을 수정
                                <br><br>
![monsteropa](https://github.com/user-attachments/assets/3a84ac96-a652-4355-9202-95e3382456a1)

      <details>
        <summary> ADefaultMonster 클래스의 BeginPlay와 OnDisappearMesh, OnDisappearMeshEnd 함수 코드 </summary>
    
     

    
       ```cpp
       /* 현재 Monster의 메시는 Opcity를 수정할 수 없는 메시로 설정되어 있습니다.
        * ADefaultMonster 클래스의 BeginPlay 함수에서
        * 멤버 변수 MaterialInstanceDynamics에 Opcity를 수정할 수 있는 Material을 저장합니다.
        * Monster 사망 시, OnDisappearMesh 함수를 호출하여
        * Monster의 메시를 MaterialInstanceDynamics에 저장된 Material로 교체하고 
        * Opcity를 조정합니다. 이로써 Monster의 메시가 점점 투명해지도록 연출할 수 있게 됩니다.
        * OnDisappearMesh 함수가 종료되면 OnDisappearMeshEnd 함수를 호출하여 Monster를 Destroy합니다.
        */
	void ADefaultMonster::BeginPlay()
	{
		Super::BeginPlay();
		
		SetData(DataTableRowHandle);
	
		USkeletalMeshComponent* SkeletalMeshComponent = GetComponentByClass<USkeletalMeshComponent>();
	
		// 1번 인덱스에 있는 Material이 BlendMode가 Translucent로 설정되어, Opcity를 수정할 수 있는 Material이다.
		// MaterialInstanceDynamics는 1번 인덱스에 있는 Material을 가리키는 멤버 변수
		MaterialInstanceDynamics = SkeletalMeshComponent->CreateAndSetMaterialInstanceDynamic(1);
		ensureMsgf(MaterialInstanceDynamics->GetBlendMode() == EBlendMode::BLEND_Translucent && MaterialInstanceDynamics, TEXT("1번 인덱스에 존재하는 Material이 없거나, BlendMode가 Translucent가 아닙니다."));
	
		if (DisappearCurve)	// DisappearCurve 값 세팅
		{
			// 몬스터 사망 모션은 하나로 함
			float DieMontagePlayLength = MonsterData->DieMontage->GetPlayLength();
			
			// 키프레임 추가
			FKeyHandle KeyHandle1 = DisappearCurve->FloatCurve.AddKey(0.0f, 1.0f);  // 시간 0에서 값 1
			// 값이 점점 증가하는 Curve
			FKeyHandle KeyHandle3 = DisappearCurve->FloatCurve.AddKey(DieMontagePlayLength, DieMontagePlayLength);  // 시간 DieMontagePlayLength에서 값 DieMontagePlayLength
	
			DisappearCurve->FloatCurve.SetKeyInterpMode(KeyHandle1, RCIM_Cubic);  // 선형 보간
			DisappearCurve->FloatCurve.SetKeyInterpMode(KeyHandle3, RCIM_Cubic);
		}
		FOnTimelineFloat Delegate;
		Delegate.BindDynamic(this, &ThisClass::OnDisappearMesh);
		// Delegate와 연동된, 즉 OnDisappearMesh 함수가 호출될 때 DisappearCurve를 인자로 넘긴다.
		DisappearTimelineComponent->AddInterpFloat(DisappearCurve, Delegate);
	
		FOnTimelineEvent EndDelegate;
		EndDelegate.BindDynamic(this, &ThisClass::OnDisappearMeshEnd);
		DisappearTimelineComponent->SetTimelineFinishedFunc(EndDelegate);
	
		...
	}

	void ADefaultMonster::OnDisappearMesh(float InDissolve)
	{
		if (MaterialInstanceDynamics)
		{
			USkeletalMeshComponent* SkeletalMeshComponent = GetComponentByClass<USkeletalMeshComponent>();
			// MaterialInstanceDynamics가 가리키고 있는, BlendMode가 Translucent로 설정된
			// Material을 Monster의 0번 Material로 설정해 준다.
			SkeletalMeshComponent->SetMaterial(0, MaterialInstanceDynamics);
	
			// CurrentTransparency의 초깃값은 1이다.
			float CurrentTransparency;
			MaterialInstanceDynamics->GetScalarParameterValue(FName("Opacity"), CurrentTransparency);
	
			float SpeedMultiplier = 0.005f; // 낮을수록 투명도 감소 속도를 더 천천히 만듭니다.
			
			// CurrentTransparency의 값을 점점 감소시켜 몬스터가 점점 투명해지도록 한다.
			float NewTransparency = FMath::Max(CurrentTransparency - InDissolve * SpeedMultiplier, 0.0f); // Max 함수는 첫 번째 인자의 값이 음수가 나오면 0.0f을 반환해 준다.
			MaterialInstanceDynamics->SetScalarParameterValue(FName("Opacity"), NewTransparency);
		}
	}
	
	void ADefaultMonster::OnDisappearMeshEnd()
	{
		Destroy();
	}
	```
	</details>
                              <br>
                              
### 🖥️ UI
  - Player 사망 시 리스폰창 출력 및 리스폰 진행
                                <br><br>
![deathAndRespawn](https://github.com/user-attachments/assets/8f044cc1-90b6-4b21-a080-380afc884a2f)
 <br><br>
 
### 📊 데이터 관리
  * ***Player의 데이터***는 언리얼 엔진 에디터상에서 데이터 테이블로 세팅 가능
          <br><br>
  ![alt text](README_content/pawndata.png "Title Text")<br>  　　　　　　  　　　　　　  　　　　　　　**`Player 데이터 테이블`**<br><br><br>
	  
  * ***Weapon의 데이터***는 언리얼 엔진 에디터상에서 데이터 테이블로 세팅 가능
    <br><br>
***Weapon 데이터 테이블***에서는 해당 Weapon이 보유할 Skill을 세팅해 줄 수 있습니다.<br><br>
![alt text](README_content/WeaponTable.png "Title Text")<br>  　　　　　　  　　　　　　  **`Weapon 데이터 테이블`**<br><br><br>
***Skill 데이터 테이블***에서는 해당 Skill 사용 시 발사되는 Projectile을 세팅해 줄 수 있습니다.<br><br> ![alt text](README_content/SkillTable.png "Title Text")<br>  　　　　　　  　　　　　　  **`Skill 데이터 테이블`**<br><br><br>
***Projectile 데이터 테이블***에서는 해당 Projectile의 크기와 스폰 위치를 세팅해 줄 수 있습니다.<br><br> ![alt text](README_content/ProjectileTable.png "Title Text")<br>  　　　　　　  　　　　　　  **`Projectile 데이터 테이블`**<br><br><br>
***Effect 데이터 테이블***에서는 Projectile의 효과음과 Effect를 세팅해 줄 수 있습니다.<br><br> ![alt text](README_content/EffectTable.png "Title Text")<br>  　　　　　　  　　　　　　    　　　**`Effect 데이터 테이블`**<br>

---

## 📝 참고 사항
- 빌드/실행 오류 발생 시 Content 경로 확인 필수
  
---

### ✉️ Contact

- 이메일 : uoipoip@gmail.com
- GitHub : [WJMcode/MBFactorio](https://github.com/WJMcode/MBFactorio)

---

