#include "AweSkillManage/AweSkillManageComponent.h"
#include <AwesomeSkillSettings.h>
#include "AweSkillManage/AwsSkillDataDefinition.h"
#include "SkillTemplate/AweSkillTemplateBase.h"
#include "Net/UnrealNetwork.h"

UAweSkillManageComponent::UAweSkillManageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UAweSkillManageComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAweSkillManageComponent, ActiveHeroAbilities);
	DOREPLIFETIME(UAweSkillManageComponent, PassiveHeroAbilities);
	DOREPLIFETIME(UAweSkillManageComponent, ActiveAbilityHandleMap);
	DOREPLIFETIME(UAweSkillManageComponent, PassiveAbilityHandleMap);
}

void UAweSkillManageComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAweSkillManageComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UAweSkillManageComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//本类继承了UGameplayTasksComponent，它的GetShouldTick函数会判断是否需要Tick，所以这里不是每帧都会进来的
	//需要override GetShouldTick 并在这个函数里面增加需要Tick的情况
	for (int i = 0; i < TickAweSkillInstances.Num(); i++)
	{
		if (TickAweSkillInstances[i].Get())
		{
			TickAweSkillInstances[i].Get()->Tick(DeltaTime);
		}
	}
}

bool UAweSkillManageComponent::GetShouldTick() const
{
	bool bResult = Super::GetShouldTick();
	if (bResult == false)
	{
		bResult=TickAweSkillInstances.Num() > 0;
	}

	return bResult;
}

UAweSkillTemplateDataBase* UAweSkillManageComponent::GetAwsSkillDataAssetByID(int32 SkillID)
{
	TObjectPtr<UDataTable> SkillsDT = nullptr;
	const TObjectPtr<UAwesomeSkillSettings> Settings = GetMutableDefault<UAwesomeSkillSettings>();
	if (!Settings)
	{
		UE_LOG(LogAweSomeSkill, Warning, TEXT("Settings are NULL! So Data Asset return nullptr;"));
	}
	
	if (Settings->SkillsDataTable.IsValid())
	{
		SkillsDT = Settings->SkillsDataTable.LoadSynchronous();
	}
	
	if (!SkillsDT)
	{
		UE_LOG(LogAweSomeSkill, Log, TEXT("Skills data table is NULL(Maybe no config)! So Data Asset return nullptr;"));
		return nullptr;
	}
	UAweSkillTemplateDataBase* DAResPtr = nullptr;
	auto TDFuncForEach = [this, &DAResPtr, &SkillID](const FName& Key, const FAwsSkillDataRow& Value)
	{
		if (Value.SkillTemplateDA.SkillID == SkillID && Value.SkillTemplateDA.SkillTemplateDA)
		{
			DAResPtr = Value.SkillTemplateDA.SkillTemplateDA;
		}
	};
	SkillsDT->ForeachRow<FAwsSkillDataRow>(TEXT("UAweSkillManageComponent::GetAwsSkillDataAssetByID"),
	                                       TDFuncForEach);
	return DAResPtr;
}

void UAweSkillManageComponent::AddTickAweSkillInstances(UAweSkillTemplateBase* SkillInstances) 
{
	TickAweSkillInstances.Add(SkillInstances);
	if (TickAweSkillInstances.Num() == 1)
	{
		UpdateShouldTick();
	}
}
void UAweSkillManageComponent::RemoveTickAweSkillInstances(UAweSkillTemplateBase* SkillInstances) 
{
	TickAweSkillInstances.Remove(SkillInstances);
	if (TickAweSkillInstances.Num() == 0)
	{
		UpdateShouldTick();
	}
}

