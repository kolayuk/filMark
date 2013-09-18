/*
 * const.h
 *
 *  Created on: 13.07.2011
 *      Author: mvideo
 */

#ifndef CONST_H_
#define CONST_H_

const TInt KCheckTimerPeriod=1000;
const TInt KCheckTimerPeriodWhileNotInit=1000;
const TInt KDeleteBeforeEnd=10;
_LIT(KConfigPath,"C:\\System\\Apps\\filMark\\config.ini");
_LIT(KAutostartPath,"C:\\System\\Apps\\filMark\\autostart.ini");
const TUid KVideoPlayerUid=TUid::Uid(0x200159b2);
const TUid  KMusicPlayerUid = TUid::Uid(0x102072c3);
//const TUid KNullUid=TUid::Uid(0x00000000);
_LIT(KConfigSeparator," > ");

#endif /* CONST_H_ */
