// Fill out your copyright notice in the Description page of Project Settings.


#include "FileToBase64Uploader_Plugin.h"
#include "HTTPModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/Base64.h"
#include "JsonParseLibrary_Plugin.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/FileHelper.h"

TArray<uint8> FStringToUint8(const FString& InString)
{
	TArray<uint8> OutBytes;

	// Handle empty strings
	if (InString.Len() > 0)
	{
		FTCHARToUTF8 Converted(*InString); // Convert to UTF8
		OutBytes.Append(reinterpret_cast<const uint8*>(Converted.Get()), Converted.Length());
	}

	return OutBytes;
}

//UFileToBase64Uploader_Plugin::UFileToBase64Uploader_Plugin()
//{
//
//}

FString UFileToBase64Uploader_Plugin::UploadFile(FString FullFilePath)
{
	FString FileName = FPaths::GetCleanFilename(FullFilePath);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *ExtensionString(FileName));

	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> HttpRequest = HttpModule.CreateRequest();

	TArray<uint8> FileRawData;
	FFileHelper::LoadFileToArray(FileRawData, *FullFilePath);


	FString Base64EncodedString = FBase64::Encode(FileRawData, EBase64Mode::Standard);
	TMap<FString, FString> EncodedData;
	EncodedData.Add("fileName", Base64EncodedString);

	if (*ExtensionString(FileName) == FString("wav"))
	{
		// ������ ��� modelName�� �ʿ���
		//UE_LOG(LogTemp, Warning, TEXT("WAV Check"));
		FString ModelName = "dahyun";
		EncodedData.Add("modelName", ModelName);
	}

	FString SendJsonData = UJsonParseLibrary_Plugin::MakeJson(EncodedData);

	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, SendJsonData);

	// �����ϴ� ���� Ȯ���ڰ� mp4�� ��� Video ��η� ����
	if (*ExtensionString(FileName) == FString("mp4"))
	{
		HttpRequest->SetURL("http://192.168.0.9:8080/view/video");
	}

	// �����ϴ� ���� Ȯ���ڰ� wav�� ��� voice ��η� ����
	if (*ExtensionString(FileName) == FString("wav"))
	{
		HttpRequest->SetURL("http://192.168.0.9:8080/view/voice");
	}

	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(SendJsonData);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UFileToBase64Uploader_Plugin::OnPostData);

	HttpRequest->ProcessRequest();

	return Base64EncodedString;
	/*FTimerHandle Timer;

	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UFileToBase64Uploader_Plugin::GetResult, 10.f, false);*/
}

void UFileToBase64Uploader_Plugin::OnPostData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		// After Upload Audio, Get a Response from Server
		UJsonParseLibrary_Plugin* jsonParser = NewObject<UJsonParseLibrary_Plugin>();
		FString res = Response->GetContentAsString();
		FString parsedData = jsonParser->JsonParse(res);

		if(!parsedData.IsEmpty())
		UE_LOG(LogTemp, Warning, TEXT("Successfully Get WAV Response : %s"), *parsedData);
	}

	//if(bConnectedSuccessfully)
	//{
	//	FString receivedData = Response->GetContentAsString();

	//}
	//else
	//{
	//	// ��û ���� ���� Ȯ��
	//	EHttpRequestStatus::Type status = Request->GetStatus();
	//	switch (status)
	//	{
	//	case EHttpRequestStatus::NotStarted:
	//		break;
	//	case EHttpRequestStatus::Processing:
	//		break;
	//	case EHttpRequestStatus::Failed:
	//		break;
	//	case EHttpRequestStatus::Failed_ConnectionError:
	//		break;
	//	case EHttpRequestStatus::Succeeded:
	//		break;
	//	default:
	//		break;
	//	}

	//	// ���� �ڵ� Ȯ��
	//	int32 responseCode = Response->GetResponseCode();
	//}
}

void UFileToBase64Uploader_Plugin::GetResult()
{
	UE_LOG(LogTemp, Error, TEXT("Response Idea"));
}

void UFileToBase64Uploader_Plugin::ProcessResponse(FString ResponseContent)
{
	UE_LOG(LogTemp, Error, TEXT("Response: %s"), *ResponseContent);

}

FString UFileToBase64Uploader_Plugin::ExtensionString(FString FilePathString)
{
	FText FileText = FText::FromString(FilePathString);

	FString FileString = FileText.ToString();

	int32 SearchPoint = UKismetStringLibrary::FindSubstring(FileString, ".") + 1;

	FString Extension = UKismetStringLibrary::RightChop(FileString, SearchPoint);

	return Extension;
}