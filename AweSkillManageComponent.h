#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AweSkillManageComponent.generated.h"

class UAweSkillTemplateBase;

UCLASS(ClassGroup = (AwsSkill), meta = (BlueprintSpawnableComponent))
class AWESOMESKILL_API UAweSkillManageComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UAweSkillManageComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool GetShouldTick() const override;

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// Get Data Asset from DT By Skill ID
	UFUNCTION(BlueprintCallable)
	UAweSkillTemplateDataBase* GetAwsSkillDataAssetByID(int32 SkillID);

protected:
	UPROPERTY()
	TArray<TObjectPtr<UAweSkillTemplateBase>> TickAweSkillInstances;

public:
	void AddTickAweSkillInstances(UAweSkillTemplateBase* SkillInstances);
	void RemoveTickAweSkillInstances(UAweSkillTemplateBase* SkillInstances);

	//获取是否队友
	virtual bool GetIsTeammate(APawn* Other) { return false; }
	//获取是否敌人
	virtual bool GetIsEnemy(APawn* Other) { return false; }

	virtual bool GetIsPlayer(APawn * Other) { return false; }

	virtual bool GetIsNpc(APawn* Other) { return false; }
	
	UPROPERTY(Replicated)
	TArray<int32> ActiveHeroAbilities;

	UPROPERTY(Replicated)
	TArray<int32> PassiveHeroAbilities;

	UPROPERTY(Replicated)
	TArray<FGameplayAbilitySpecHandle> ActiveAbilityHandleMap;

	UPROPERTY(Replicated)
	TArray<FGameplayAbilitySpecHandle> PassiveAbilityHandleMap;
	
};



