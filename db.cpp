/** when u insert larage data to db, u can use transaction for getting more speed */
try
	{
		//if (!DB->Open(TEXT("ODBC;DSN=AntiDB"), NULL, NULL))
		if (!DB->Open(TEXT("DRIVER={MySQL ODBC 8.0 Unicode Driver};SERVER=192.168.1.251;DATABASE=Anti;UID=root;PASSWORD=root;OPTION=3"), NULL, NULL))
			return false;

		//FString sql = FString::Printf(TEXT("select * from View_Login where UserName=%s and PassWord=%s"), *Message.UserName, *Message.Password);

		// Using transaction
		DB->Execute(TEXT("START TRANSACTION"));

		FString sql;
		for (int i = 0; i < 50000; ++i)
		{
			DB->Execute(*FString::Printf(TEXT("insert into test values(%d, 'dshgidsjflds');\n"), i));
		}

		DB->Execute(TEXT("commit;"));

		sql = FString::Printf(TEXT("select * from test"));
		FDataBaseRecordSet* RecordSet;
		if (DB->Execute(*sql, RecordSet))
		{
			FDataBaseRecordSet::TIterator it(RecordSet);
			for (; it; ++it)
			{
				UE_LOG(LogDataServer, Display, TEXT("test id is: %d"), it->GetInt(TEXT("idtest")));
			}
		}
	}
	catch (_com_error &e)
	{
		UE_LOG(LogDataServer, Display, TEXT("Incoming ping message: %s"), e.ErrorMessage());
	}
