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

void UFileToBase64Uploader_Plugin::UploadFile(FString FullFilePath)
{
	FString FileName = FPaths::GetCleanFilename(FullFilePath);

	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> HttpRequest = HttpModule.CreateRequest();

	TArray<uint8> FileRawData;
	FFileHelper::LoadFileToArray(FileRawData, *FullFilePath);

	FString Base64EncodedString = FBase64::Encode(FileRawData, EBase64Mode::Standard);
	TMap<FString, FString> EncodedData;
	EncodedData.Add("fileName", Base64EncodedString);

	FString SendJsonData = UJsonParseLibrary_Plugin::MakeJson(EncodedData);

	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, SendJsonData);

	HttpRequest->SetURL("http://192.168.0.12:8080/view/video");
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(SendJsonData);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UFileToBase64Uploader_Plugin::OnPostData);

	HttpRequest->ProcessRequest();
}

void UFileToBase64Uploader_Plugin::OnPostData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	//if (bConnectedSuccessfully)
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

void UFileToBase64Uploader_Plugin::ProcessResponse(FString ResponseContent)
{
	UE_LOG(LogTemp, Error, TEXT("Response: %s"), *ResponseContent);
}
