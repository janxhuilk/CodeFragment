// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreTypes.h"
#include "Containers/Map.h"
#include "Delegates/IDelegateInstance.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "IMessageContext.h"
#include "Misc/Guid.h"
#include "Templates/SharedPointer.h"
#include "DataMessages.h"

class FMessageEndpoint;
class FDataBaseConnection;

class FDataServer
{
public:
	FDataServer();
	virtual ~FDataServer();

public:
	
	virtual bool IsRunning()
	{
		return MessageEndpoint.IsValid();
	}

	virtual bool Start();
	virtual void Stop();

	/**
	* Publishes a ping response.
	*
	* @param Context The context of the received Ping message.
	* @param UserName The name of the user that sent the ping.
	*/
	void SendPong(const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context, const FString& UserName);

protected:

	void AllowLogin(const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);

private:

	/** Callback for FMessageRpcProgress messages. */
	void HandleProgressUserInfoMessage(const FDataUserInfo& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);

	/** Handles FSessionServicePing messages. */
	void HandlePingMessage(const FDataPing& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);

	/** Handles all incoming messages. */
	void HandleMessage(const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);

	void HandleLoginMessage(const FDataLogin& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);

private:

	/** Message endpoint. */
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> MessageEndpoint;
};