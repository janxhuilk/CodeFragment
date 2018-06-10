// Fill out your copyright notice in the Description page of Project Settings.

#include "DataServer.h"
#include "Networking.h"
#include "MessageEndpointBuilder.h"
#include "DataBase.h"
#include "WindowsHWrapper.h"
#include "AllowWindowsPlatformTypes.h"
#include "AllowWindowsPlatformAtomics.h"
#include "comdef.h"

DEFINE_LOG_CATEGORY_STATIC(LogDataServer, Log, All);

/* FDataServer structors
*****************************************************************************/
FDataServer::FDataServer()
{
	DB = FDataBaseConnection::CreateObject();
}

FDataServer::~FDataServer()
{
	Stop();
	DB->Close();
	delete DB;
	DB = nullptr;
}

//*****************************************************************************/

bool FDataServer::Start()
{
	MessageEndpoint = FMessageEndpoint::Builder("FMessageRpcClient")
		.Handling<FDataPing>(this, &FDataServer::HandlePingMessage)
		.Handling<FDataLogin>(this, &FDataServer::HandleLoginMessage)
		.Handling<FDataUserInfo>(this, &FDataServer::HandleProgressUserInfoMessage)
		.WithCatchall(this, &FDataServer::HandleMessage);

	if (!MessageEndpoint.IsValid())
	{
		return false;
	}

	MessageEndpoint->Subscribe<FDataPing>();

	return true;
}

void FDataServer::Stop()
{
	if (IsRunning())
	{
		MessageEndpoint.Reset();
	}
}

void FDataServer::SendPong(const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context, const FString& UserName)
{
	if (!MessageEndpoint.IsValid())
	{
		return;
	}

	FDataPong* Message = new FDataPong();
	{
		Message->Authorized = FApp::IsAuthorizedUser(UserName);
		Message->BuildDate = FApp::GetBuildDate();
		Message->DeviceName = FPlatformProcess::ComputerName();
		Message->InstanceId = FApp::GetInstanceId();
		Message->InstanceName = FApp::GetInstanceName();

#if PLATFORM_DESKTOP
		Message->IsConsoleBuild = false;
#else
		Message->IsConsoleBuild = true;
#endif

		Message->PlatformName = FPlatformProperties::PlatformName();
		Message->SessionId = FApp::GetSessionId();
		Message->SessionName = FApp::GetSessionName();
		Message->SessionOwner = FApp::GetSessionOwner();
		Message->Standalone = FApp::IsStandalone();
	}

	MessageEndpoint->Send(Message, Context->GetSender());
}

void FDataServer::AllowLogin(const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	if (!MessageEndpoint.IsValid())
	{
		return;
	}

	FDataAllowLogin* AllowLogin = new FDataAllowLogin();
	{
		AllowLogin->UserId = 15646131;
		AllowLogin->UserName = TEXT("你好AI");
	}

	MessageEndpoint->Send(AllowLogin, Context->GetSender());
}

/* FDataServer event handlers
*****************************************************************************/

void FDataServer::HandleProgressUserInfoMessage(const FDataUserInfo& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Incoming new message: %s"), *Message.UserName));
	UE_LOG(LogDataServer, Display, TEXT("Incoming new message: %s"), *Message.UserName);
}

void FDataServer::HandlePingMessage(const FDataPing& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Incoming ping message: %s"), *Message.UserName));
	UE_LOG(LogDataServer, Display, TEXT("Incoming ping message: %s"), *Message.UserName);
	SendPong(Context, Message.UserName);
}

void FDataServer::HandleMessage(const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Incoming ping message: %s"), *Context->GetMessageType().ToString()));
	UE_LOG(LogDataServer, Display, TEXT("Incoming ping message: %s"), *Context->GetMessageType().ToString());
}

void FDataServer::HandleLoginMessage(const FDataLogin& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Incoming Login message: %s, %s"), *Message.UserName, *Message.Password);
}