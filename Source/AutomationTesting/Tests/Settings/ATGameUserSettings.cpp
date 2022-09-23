// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests/Settings/ATGameUserSettings.h"

#include "ATGameSetting.h"
#include "Internationalization/StringTableRegistry.h"

#define LOCTEXT_NAMESPACE "GameUserSettings"

UATGameUserSettings::UATGameUserSettings()
{
	const FName NameTable = "/Game/AutomationTesting/Data/ST_MyLocal.ST_MyLocal";
	TArray<FString> QualityTable = {"Low_Loc", "Medium_Loc", "High_Loc", "Epic_Loc"};
	TArray<FATSettingOption> SettingQualityOptions;
	for (int32 i = 0; i < QualityTable.Num(); ++i)
	{
		SettingQualityOptions.Add({FText::FromStringTable(NameTable, QualityTable[i]), i});
	}

	//считаем что this живет на всем протяжении

	TArray<FString> SettingNameQualityTable = {"Anti-Aliasing_Loc", "Textures_Loc", "GlobalIllumination_Loc", "Shadows_Loc",
	                                           "PostProcessing_Loc"};

	GraphicsSettings.Add(&NewObject<UATGameSetting>()->SetName(FText::FromStringTable(NameTable, SettingNameQualityTable[0]))
	                                                 .SetOption(SettingQualityOptions)
	                                                 .SetGetter([this]() { return GetAntiAliasingQuality(); })
	                                                 .SetSetter([this](int32 Quality)
	                                                 {
		                                                 SetAntiAliasingQuality(Quality);
		                                                 ApplySettings(false);
	                                                 }));

	GraphicsSettings.Add(&NewObject<UATGameSetting>()->SetName(FText::FromStringTable(NameTable, SettingNameQualityTable[1]))
	                                                 .SetOption(SettingQualityOptions)
	                                                 .SetGetter([this]() { return GetTextureQuality(); })
	                                                 .SetSetter([this](int32 Quality)
	                                                 {
		                                                 SetTextureQuality(Quality);
		                                                 ApplySettings(false);
	                                                 }));

	GraphicsSettings.Add(&NewObject<UATGameSetting>()->SetName(FText::FromStringTable(NameTable, SettingNameQualityTable[2]))
	                                                 .SetOption(SettingQualityOptions)
	                                                 .SetGetter([this]() { return GetGlobalIlluminationQuality(); })
	                                                 .SetSetter([this](int32 Quality)
	                                                 {
		                                                 SetGlobalIlluminationQuality(Quality);
		                                                 ApplySettings(false);
	                                                 }));

	GraphicsSettings.Add(&NewObject<UATGameSetting>()->SetName(FText::FromStringTable(NameTable, SettingNameQualityTable[3]))
	                                                 .SetOption(SettingQualityOptions)
	                                                 .SetGetter([this]() { return GetShadowQuality(); })
	                                                 .SetSetter([this](int32 Quality)
	                                                 {
		                                                 SetShadowQuality(Quality);
		                                                 ApplySettings(false);
	                                                 }));

	GraphicsSettings.Add(&NewObject<UATGameSetting>()->SetName(FText::FromStringTable(NameTable, SettingNameQualityTable[4]))
	                                                 .SetOption(SettingQualityOptions)
	                                                 .SetGetter([this]() { return GetPostProcessingQuality(); })
	                                                 .SetSetter([this](int32 Quality)
	                                                 {
		                                                 SetPostProcessingQuality(Quality);
		                                                 ApplySettings(false);
	                                                 }));
}


UATGameUserSettings* UATGameUserSettings::Get()
{
	return GEngine ? Cast<UATGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

const TArray<UATGameSetting*>& UATGameUserSettings::GetGraphicsSettings() const
{
	return GraphicsSettings;
}

void UATGameUserSettings::RunBenchmark()
{
	RunHardwareBenchmark();
	ApplySettings(false);
	OnGraphicsSettingsUpdated.Broadcast();
}

#undef LOCTEXT_NAMESPACE
