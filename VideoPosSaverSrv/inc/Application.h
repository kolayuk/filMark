/*
 ============================================================================
 Name		: Application.h
 Author	  : Usanov-Kornilov Nikolay (aka Kolay)
 Version	 : 1.0
 Copyright   : 
 Contacts:
 kolayuk@mail.ru
 http://kolaysoft.ru

 (c) KolaySoft, 2011

 Description : CApplication declaration
 ============================================================================
 */

#ifndef APPLICATION_H
#define APPLICATION_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "mpxplaybackutility.h"
#include <aiplayerpluginengine.h>
#include <aiplayerpluginengineobserver.h>
#include <w32std.h>
#include <FgrObserver.h>
#include <f32file.h>



class CApplication : public CBase, public MAiPlayerPluginEngineObserver,public MFgrCallBack
	{
public:
	~CApplication();
	static CApplication* NewL();
	static CApplication* NewLC();
	void PlayerStateChanged( TMPXPlaybackState aState );
	void TrackInfoChanged( const TDesC& aTitle,const TDesC& aArtist );
	void PlaybackPositionChanged( TInt aPosition );
	void AlbumArtChanged( CFbsBitmap* aBitmap );
	void Opening();
	void PlaylisIsEmpty();
	static TInt Tick(TAny* aObj);
	void DoTick();
	void GroupListChanged();
private:
	CApplication();
	void ConstructL();
	void SetPosition();
	void SavePosition();
	void DeleteEmpty();
	CPeriodic* iTimer;
	CAiPlayerPluginEngine* iPlayerEngine;
	RWsSession* iWsSession;
	CFgrObserver* iObserver;
	RFs iFs;
	TMPXPlaybackState iState;
	TInt iPrevPos;
	};

#endif // APPLICATION_H
