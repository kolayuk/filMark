/*
 ============================================================================
 Name		: Application.cpp
 Author	  : Usanov-Kornilov Nikolay (aka Kolay)
 Version	 : 1.0
 Copyright   : 
 Contacts:
 kolayuk@mail.ru
 http://kolaysoft.ru

 (c) KolaySoft, 2011

 Description : CApplication implementation
 ============================================================================
 */

#include "Application.h"
#include <logger.h>
#include <const.h>
#include <apgtask.h>
#include <apgwgnam.h>
#include <bautils.h>
#include <e32property.h>
#include <e32math.h>

CApplication::CApplication()
	{
	// No implementation required
	}

CApplication::~CApplication()
	{
	if (iTimer->IsActive()){iTimer->Cancel();}
	if (iTimer){delete iTimer;}
	if (iPlayerEngine){delete iPlayerEngine;}
	if (iWsSession){iWsSession->Close();}
	if (iWsSession){delete iWsSession;}
	iFs.Close();
	}

CApplication* CApplication::NewLC()
	{
	CApplication* self = new (ELeave) CApplication();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CApplication* CApplication::NewL()
	{
	CApplication* self = CApplication::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CApplication::ConstructL()
	{
	_LOG(_L("Srv launched"));
	iFs.Connect();
#ifndef _DEBUG
	if (!BaflUtils::FileExists(iFs,KAutostartPath)){_LOG(_L("no autostart -> exit")); User::Exit(0);}
#endif
	DeleteEmpty();
	
	iWsSession=new (ELeave) RWsSession();
	iWsSession->Connect();
	iTimer=CPeriodic::NewL(CActive::EPriorityHigh);
	iTimer->Start(KCheckTimerPeriod*1000,KCheckTimerPeriod*1000,TCallBack(Tick,this));
	//iObserver=CFgrObserver::NewL(*iWsSession,*this);
	iPrevPos=-1;
	}

void CApplication::PlaybackPositionChanged(TInt aPos)
	{
	_LOGDATA(_L("PosChanged %d"),aPos);
	
	if (iState==EPbStateNotInitialised){iState=iPlayerEngine->PlayerState()==EPbStateNotInitialised?EPbStateNotInitialised:iPlayerEngine->PlayerState();}
	if ((iState==EPbStateNotInitialised))
		{
		_LOG(_L("Uninit - del"));
		if (iPlayerEngine)
			{
			if (iTimer->IsActive()){iTimer->Cancel();}
			delete iPlayerEngine;
			iPlayerEngine=NULL;
			iTimer->Start(KCheckTimerPeriodWhileNotInit*1000,KCheckTimerPeriodWhileNotInit*1000,TCallBack(Tick,this));
			_LOG(_L("Tmr started"));
			return;
			}
		}
		
	if (((aPos<=5&&aPos>0)&&iPlayerEngine->PlayerState()==EPbStatePlaying))
		{
		//iPlayerEngine->iPlaybackUtility->CommandL(EPbCmdPlayPause);
		if (iPrevPos<2){SetPosition();}
		//iPlayerEngine->iPlaybackUtility->CommandL(EPbCmdPlayPause);
		}
	
	if (aPos%15==0)
		{
		if (aPos>2){iPrevPos=aPos;SavePosition();}
		}
	_LOGDATA(_L("iPrevPos: %d"),iPrevPos);
	}
void CApplication::PlayerStateChanged(TMPXPlaybackState aState)
	{
	_LOGDATA(_L("PlayerStateChanged %d"),aState);
	iState=aState;
	if (aState==EPbStatePlaying)
		{
		if (iTimer->IsActive()){iTimer->Cancel();}
		iTimer->Start(KCheckTimerPeriod*1000,KCheckTimerPeriod*1000,TCallBack(Tick,this));
		}
	if (aState==EPbStatePlaying){iPlayerEngine->RequestMediaL();}
	if (aState==EPbStatePlaying&&(iPlayerEngine->Position()<5&&iPlayerEngine->Position()>0))
		{
		//iPlayerEngine->iPlaybackUtility->CommandL(EPbCmdPlayPause);
		if (iPrevPos<2){SetPosition();}
		//iPlayerEngine->iPlaybackUtility->CommandL(EPbCmdPlayPause);
		}
		
	if ((aState==EPbStatePaused||aState==EPbStateStopped)&&iPlayerEngine->Position()>2){SavePosition();}
	if (aState==EPbStateStopped){iPrevPos=-1;}
	}
void CApplication::TrackInfoChanged(const TDesC& aTitle, const TDesC& aArtist)
	{
	_LOGDATA(_L("TrackInfoChanged %S"),&iPlayerEngine->Uri());
	iState-iPlayerEngine->PlayerState();
	}
void CApplication::Opening(){}
void CApplication::AlbumArtChanged(CFbsBitmap* aBitmap){}
void CApplication::PlaylisIsEmpty(){}

TInt CApplication::Tick(TAny* aObj)
	{((CApplication*)aObj)->DoTick();}
void CApplication::DoTick()
	{
	TApaTaskList lst(*iWsSession);
	TApaTask tsk=lst.FindApp(KVideoPlayerUid);
	if (tsk.Exists())
		{
		if(!iPlayerEngine)
			{
			_LOG(_L("player opened"));
			iPrevPos=-1;
			TApaTask mPlayer=lst.FindApp(KMusicPlayerUid);
			if (!mPlayer.Exists()){iPlayerEngine=CAiPlayerPluginEngine::NewL(*this);}
			if (mPlayer.Exists()){mPlayer.EndTask();}
			}
		}
	else
		{
		if (iPlayerEngine){_LOG(_L("player closed"));delete iPlayerEngine;iPlayerEngine=NULL;}
		
		}
		
	}
void CApplication::GroupListChanged()
	{
	/*
	TApaTaskList lst(*iWsSession);
	TApaTask fgrTsk=lst.FindByPos(0);
	TApaTask plTask=lst.FindApp(KVideoPlayerUid);
	TInt wg1,wg2;
	wg1=fgrTsk.WgId();
	wg2=plTask.WgId();
	_LOGDATA2(_L("fgr task id: %d,player task id: %d"),wg1,wg2);
	if (wg1==wg2)
		{
		if(!iPlayerEngine){_LOG(_L("player opened"));iPlayerEngine=CAiPlayerPluginEngine::NewL(*this);}
		}
	else
		{
		if (iPlayerEngine){_LOG(_L("player closed"));delete iPlayerEngine;iPlayerEngine=NULL;}
		}
	*/
	}
void CApplication::SetPosition()
	{
	_LOG(_L("SetPosition"));
	if (BaflUtils::FileExists(iFs,KConfigPath)) //!!!!!!!!!!!!!!!!!!!!!!!!!!!
			{
			TBuf<255> val;
			RFile filesave;
			TBuf<10> t;
			TFileText ft;
			TUint32 IntVal;
			filesave.Open(iFs, KConfigPath, EFileRead);
			ft.Set(filesave);
			CDesC16Array* arr=new (ELeave)CDesC16ArrayFlat(3);
			TBuf<255> KSeparator(KConfigSeparator);
			while (ft.Read(val)==KErrNone)
				{
				if (val.Find(iPlayerEngine->Uri())!=-1)
					{
					arr->Reset();
					while (val.Find(KSeparator)!=KErrNone)
						{
						TInt f=val.Find(KSeparator);
						if (f==KErrNotFound){break;}
						arr->AppendL(val.Left(f));
						val.Copy(val.Right(Abs(f-val.Length()+KSeparator.Length())));
						}	
					arr->AppendL(val);
					TLex conv(arr->MdcaPoint(1));
					TInt pos;
					conv.Val(pos);
					_LOGDATA(_L("will set pos %d"),pos);
					if (iPlayerEngine){iPlayerEngine->iPlaybackUtility->SetL(EPbPropertyPosition,pos*1000);}
					break;
					}
				}
			filesave.Close();
			}
	}
void CApplication::SavePosition()
	{ //переписать на HBufC
	_LOG(_L("SavePos"));
	TBuf <768> uri=iPlayerEngine->Uri();
	if (iPlayerEngine->Uri().Length()==0){return;}
	if (uri.Find(_L("http://"))!=-1||uri.Find(_L("rtsp://"))!=-1){return;}
	TParse parse;
	iFs.Parse(KConfigPath,parse);
	if (!BaflUtils::FolderExists(iFs,parse.DriveAndPath()))
		{
		iFs.MkDirAll(parse.DriveAndPath());
		}
	CDesC16Array*full=new (ELeave)CDesC16ArrayFlat(3);
	TBuf<800> val;
	RFile filesave;
	TBuf<10> t; 
	TFileText ft;
	TUint32 IntVal;
	if (BaflUtils::FileExists(iFs,KConfigPath))
		{
		filesave.Open(iFs, KConfigPath, EFileRead);
		ft.Set(filesave);
		TBuf<10> KSeparator(KConfigSeparator);
		TBool found=EFalse;
		while (ft.Read(val)==KErrNone)
			{
			if (val.Find(iPlayerEngine->Uri())!=-1)
				{
				_LOGDATA(_L("duration %d"),iPlayerEngine->Duration());
				if (iPlayerEngine->Duration()==-1||(Abs(iPlayerEngine->Duration()-iPlayerEngine->Position())>KDeleteBeforeEnd*60))
					{
					TBuf<768> buf;
					buf.Copy(iPlayerEngine->Uri());
					buf.Append(KConfigSeparator);
					buf.AppendNum(iPlayerEngine->Position());
					full->AppendL(buf);
					found=ETrue;
					}
				else {_LOG(_L("pos del")); found=ETrue;}
				}
			else
				{
				full->AppendL(val);
				}
			
			}
		if (!found)
			{
			TBuf<768> buf;
			buf.Copy(iPlayerEngine->Uri());
			buf.Append(KConfigSeparator);
			buf.AppendNum(iPlayerEngine->Position());
			full->AppendL(buf);
			}
		filesave.Close();
		}
	else
		{
		TBuf<768> buf;
		buf.Copy(iPlayerEngine->Uri());
		buf.Append(KConfigSeparator);
		buf.AppendNum(iPlayerEngine->Position());
		full->AppendL(buf);
		}
	_LOGDATA(_L("full count %d"),full->Count());
	
	filesave.Replace(iFs, KConfigPath, EFileWrite);
	ft.Set(filesave);
	for (TInt i=0;i<full->Count();i++)
		{
		ft.Write(full->MdcaPoint(i));
		}
	filesave.Close();
	delete full;
	_LOG(_L("Saved ok"));
	}
void CApplication::DeleteEmpty()
	{
	CDesC16Array*full=new (ELeave)CDesC16ArrayFlat(3);
	TBuf<255> val;
	RFile filesave;
	TBuf<10> t;
	TFileText ft;
	TUint32 IntVal;
	if (BaflUtils::FileExists(iFs,KConfigPath))
		{
		filesave.Open(iFs, KConfigPath, EFileRead);
		ft.Set(filesave);
		TBuf<255> KSeparator(KConfigSeparator);
		TBool found=EFalse;;
		CDesCArray* arr=new (ELeave) CDesC16ArrayFlat(3);
		while (ft.Read(val)==KErrNone)
			{
			TBuf<255> txt; txt.Copy(val);
			arr->Reset();
			while (val.Find(KSeparator)!=KErrNone)
				{
				TInt f=val.Find(KSeparator);
				if (f==KErrNotFound){break;}
				arr->AppendL(val.Left(f));
				val.Copy(val.Right(Abs(f-val.Length()+KSeparator.Length())));
				}	
			arr->AppendL(val);
			if (BaflUtils::FileExists(iFs,arr->MdcaPoint(0))){full->AppendL(txt);}
			}
		delete arr;
		filesave.Close();
		}
	_LOGDATA(_L("full count %d (del)"),full->Count());
	if (full->Count()==0){BaflUtils::DeleteFile(iFs,KConfigPath);}
	else
		{
		filesave.Replace(iFs, KConfigPath, EFileWrite);
		ft.Set(filesave);
		for (TInt i=0;i<full->Count();i++)
			{
			ft.Write(full->MdcaPoint(i));
			}
		filesave.Close();
		}
	}
