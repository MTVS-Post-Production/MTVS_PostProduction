// Fill out your copyright notice in the Description page of Project Settings.


#include "SLoginWebPage.h"
#include "IPConfig.h"
#include "LoginLogic_Plugin.h"
#include "Kismet/KismetStringLibrary.h"
#include "Widgets/SCanvas.h"
#include "GenericPlatform/GenericPlatformTime.h"
#include "TimerManager.h"
#include "Containers/Ticker.h"
#include "Framework/Docking/TabManager.h"



void SLoginWebPage::Construct(const FArguments& InArgs)
{
	FString URL = IPConfig::StaticVariable + "/oauth2/authorize/kakao";

	loginWebBrowser = SNew(SWebBrowser)
		.InitialURL(URL)
		.ShowAddressBar(true)
		.OnUrlChanged_Raw(this, &SLoginWebPage::OnURLChanged);

	ChildSlot
		[
			SNew(SCanvas)

				+ SCanvas::Slot()
				.Position(FVector2D(50, 50))
				.Size(FVector2D(1000, 500))
				//.HAlign(HAlign_Center)
				//.VAlign(VAlign_Center)
				[
					loginWebBrowser.ToSharedRef()
				]

		];

}

void SLoginWebPage::OnURLChanged(const FText& InText)
{
	UE_LOG(LogTemp, Warning, TEXT("SLoginWebPage::OnURLChanged"),);
	URLString = InText.ToString();

	UE_LOG(LogTemp, Warning, TEXT("%s"), *URLString);

	static FString CheckPointString = "https://logins.daum.net/accounts/kakaossotokenlogin.do?redirect=true&ssotoken=";
	//static FString NextStepString = "http://192.168.1.11:8080/oauth2/callback/kakao?code=";
	static FString NextStepString = IPConfig::StaticVariable;

	//bool NeedValueString = UKismetStringLibrary::Contains(URLString, NextStepString);
	bool NeedValueString = UKismetStringLibrary::Contains(URLString, NextStepString);

	if (NeedValueString && DoOnceBool == false)
	{
		tikcerHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &SLoginWebPage::OnTicker), 1.0f);
	}
}


bool SLoginWebPage::OnTicker(float DeltaTime)
{

	auto Callback = [=](const FString& SourceURL) {
		// Handle the source URL as needed
		UE_LOG(LogTemp, Warning, TEXT("Source URL1 : %s"), *SourceURL);
		ParsingHtml(SourceURL);
		};

	loginWebBrowser->GetSource(Callback);

	FTicker::GetCoreTicker().RemoveTicker(tikcerHandle);

	return true;

}

void SLoginWebPage::ParsingHtml(FString HtmlString)
{
	
	HtmlString.Split(TEXT("{"), nullptr, &HtmlString, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	HtmlString.Split(TEXT(":"), nullptr, &HtmlString, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	HtmlString.Split(TEXT("\""), nullptr, &HtmlString, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	HtmlString.Split(TEXT("\""), &HtmlString, nullptr, ESearchCase::IgnoreCase, ESearchDir::FromStart);

	UE_LOG(LogTemp, Warning, TEXT("Access Token : %s"), *HtmlString);
	IPConfig::Token = HtmlString;

	// ȭ�� ��ȯ
	ConvertTab();
}

void SLoginWebPage::ConvertTab()
{	
	FGlobalTabmanager::Get()->TryInvokeTab(FName("Video Tab"));
	FGlobalTabmanager::Get()->TryInvokeTab(FName("Sound Tab"));
	FGlobalTabmanager::Get()->TryInvokeTab(FName("Motion Tab"));

	// Close the old tab if it exists
	TSharedPtr<SDockTab> OldTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FName("Login Tab"));
	if (OldTab.IsValid())
	{
		OldTab->RequestCloseTab();
	}
}
