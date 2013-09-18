/*
 ============================================================================
 Name		: VideoPosSaverSrv.cpp
 Author	  : Usanov-Kornilov Nikolay (aka Kolay)
 Copyright   : 
 Contacts:
 kolayuk@mail.ru
 http://kolaysoft.ru

 (c) KolaySoft, 2011

 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "VideoPosSaverSrv.h"
#include <e32base.h>
#include <e32std.h>
#include <Application.h>
LOCAL_C void MainL()
	{
	CApplication* app=CApplication::NewL();
	CActiveScheduler::Start();
	delete app;
	}

LOCAL_C void DoStartL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	MainL();
	CleanupStack::PopAndDestroy(scheduler);
	}

//  Global Functions

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TRAPD(mainError, DoStartL());
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}

