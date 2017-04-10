//·ûºÅ³£Á¿ Ê®Áù½øÖÆÖµ Ö¸¶¨µÄÊó±ê»ò¼üÅÌ°´¼ü
//		;		//		;		//		;		//		;		// VMKey.h
#include <afxwin.h>
const long VK_0=48;
const long VK_1=49;
const long VK_2=50;
const long VK_3=51;
const long VK_4=52;
const long VK_5=53;
const long VK_6=54;
const long VK_7=55;
const long VK_8=56;
const long VK_9=57;
const long VK_A=65;
const long VK_B=66;
const long VK_C=67;
const long VK_D=68;
const long VK_E=69;
const long VK_F=70;
const long VK_G=71;
const long VK_H=72;
const long VK_I=73;
const long VK_J=74;
const long VK_K=75;
const long VK_L=76;
const long VK_M=77;
const long VK_N=78;
const long VK_O=79;
const long VK_P=80;
const long VK_Q=81;
const long VK_R=82;
const long VK_S=83;
const long VK_T=84;
const long VK_U=85;
const long VK_V=86;
const long VK_W=87;
const long VK_X=88;
const long VK_Y=89;
const long VK_Z=90;


long KeyList(CString string)
{
if(string=="MouseLeft") return VK_LBUTTON;		
else if(string=="MouseRight") return VK_RBUTTON;		
else if(string=="MouseMid") return VK_MBUTTON;
else if(string=="BACKSPACE") return VK_BACK;	//08		;		// ¼ü 
else if(string=="TAB") return VK_TAB;		//09		;		// ¼ü 
else if(string=="CLEAR") return VK_CLEAR		;		//0C		;		// ¼ü 
else if(string=="ENTER") return VK_RETURN		;		//0D		;		// ¼ü 
//else if(string=="SHelse") return VK_SHelse  ;		//10		;		// ¼ü 
else if(string=="CTRL") return VK_CONTROL		;		//11		;		// ¼ü 
else if(string=="ALT") return VK_MENU		;		//12		;		// ¼ü 
else if(string=="PAUSE") return VK_PAUSE 		;		// ¼ü 
else if(string=="CAPSLOCK") return VK_CAPITAL		;		//14		;		// ¼ü		;		//
else if(string=="ESC") return VK_ESCAPE		;		//1B		;		// ¼ü 
else if(string=="SPACEBAR") return VK_SPACE		;		//20		;		//		;		//
else if(string=="PAGEUP") return VK_PRIOR		;		//21		;		// ¼ü		;		//
else if(string=="PAGEDOWN") return VK_NEXT		;		//22		;		// ¼ü 
else if(string=="END") return VK_END		;		//23		;		// ¼ü 
else if(string=="HOME") return VK_HOME		;		//24		;		// ¼ü 
else if(string=="LEFTARROW") return VK_LEFT		;		//25		;		// ¼ü		;		//
else if(string=="UPARROW") return VK_UP		;		//26		;		// ¼ü		;		//
else if(string=="RIGHTARROW") return VK_RIGHT		;		//27		;		// ¼ü		;		//
else if(string=="DOWNARROW") return VK_DOWN		;		//28		;		// ¼ü		;		//
else if(string=="SELECT") return VK_SELECT		;		//29		;		// ¼ü 
else if(string=="EXECUTE") return VK_EXECUTE		;		//2B		;		// ¼ü		;		//
else if(string=="PRSCREEN") return VK_SNAPSHOT		;		//2C		;		//¼ü£¨ÓÃÓÚWindows 3.0¼°ÒÔºó°æ±¾£© 
else if(string=="INS") return VK_INSERT		;		//2D		;		// ¼ü		;		//
else if(string=="DEL") return VK_DELETE		;		//2E		;		// ¼ü		;		//
else if(string=="HELP") return VK_HELP		;		//2F		;		// ¼ü		;		//
else if(string=="0") return VK_0		;		//30		;		//0 ¼ü		;		//
else if(string=="1") return VK_1		;		//31		;		//1 ¼ü		;		//
else if(string=="2") return VK_2		;		//32		;		//2 ¼ü		;		//
else if(string=="3") return VK_3		;		//33		;		//3 ¼ü		;		//
else if(string=="4") return VK_4		;		//34		;		//4 ¼ü		;		//
else if(string=="5") return VK_5		;		//35		;		//5 ¼ü		;		//
else if(string=="6") return VK_6		;		//36		;		//6 ¼ü		;		//
else if(string=="7") return VK_7		;		//37		;		//7 ¼ü		;		//
else if(string=="8") return VK_8		;		//38		;		//8 ¼ü		;		//
else if(string=="9") return VK_9		;		//39		;		//9 ¼ü 
else if(string=="A") return VK_A		;		//41		;		//A ¼ü		;		//
else if(string=="B") return VK_B		;		//42		;		//B ¼ü		;		//
else if(string=="C") return VK_C		;		//43		;		//C ¼ü		;		//
else if(string=="D") return VK_D		;		//44		;		//D ¼ü		;		//
else if(string=="E") return VK_E		;		//45		;		//E ¼ü		;		//
else if(string=="F") return VK_F		;		//46		;		//F ¼ü		;		//
else if(string=="G") return VK_G		;		//47		;		//G ¼ü		;		//
else if(string=="H") return VK_H		;		//48		;		//H ¼ü 
else if(string=="I") return VK_I		;		//49		;		//I ¼ü 
else if(string=="J") return VK_J		;		// 4A 		;		// J ¼ü 
else if(string=="K") return VK_K		;		// 4B 		;		// K ¼ü 
else if(string=="L") return VK_L 		;		// 4C 		;		// L ¼ü
else if(string=="M") return VK_M 		;		// 4D 		;		// M ¼ü
else if(string=="N") return VK_N		;		// 4E 		;		//¼ü
else if(string=="O") return VK_O 		;		// 4F 		;		// O ¼ü
else if(string=="P") return VK_P 		;		// 		;		//50 		;		// P ¼ü
else if(string=="Q") return VK_Q 		;		// 51 		;		// Q ¼ü
else if(string=="R") return VK_R 		;		// 52 		;		// R ¼ü
else if(string=="S") return VK_S 		;		//53 		;		//S ¼ü
else if(string=="T") return VK_T 		;		//54 		;		//T ¼ü
else if(string=="U") return VK_U 		;		// 55 		;		// U ¼ü
else if(string=="V") return VK_V 		;		// 56 		;		// V ¼ü
else if(string=="W") return VK_W 		;		// 57 		;		// W ¼ü
else if(string=="X") return VK_X 		;		// 58 		;		//X ¼ü
else if(string=="Y") return VK_Y 		;		// 59 		;		// Y ¼ü
else if(string=="Z") return VK_Z 		;		// 5A 		;		// Z ¼ü
//else if(string=="Left Windows") return VK_LWI 		;		// 5B 		;		// Left Wi dows ¼ü (Microsoft×ÔÈ»¼üÅÌ)
//else if(string=="Right Windows") return VK_RWI 		;		// 5C 		;		// Right Wi dows ¼ü (Microsoft×ÔÈ»¼üÅÌ)
else if(string=="Applications") return VK_APPS 		;		// 5D 		;		// Applicatio s ¼ü (Microsoft×ÔÈ»¼üÅÌ)
else if(string=="NUM0") return VK_NUMPAD0 		;		// 60 		;		// Êý×ÖÐ¡¼üÅÌÉÏµÄ 0 ¼ü
else if(string=="NUM1") return VK_NUMPAD1 		;		// 61 		;		// Êý×ÖÐ¡¼üÅÌÉÏµÄ 1 ¼ü
else if(string=="NUM2") return VK_NUMPAD2 		;		// 62 		;		// Êý×ÖÐ¡¼üÅÌÉÏµÄ 2 ¼ü
else if(string=="NUM3") return VK_NUMPAD3		;		//63		;		//Êý×ÖÐ¡¼üÅÌÉÏµÄ 3 ¼ü
else if(string=="NUM4") return VK_NUMPAD4		;		//64		;		//Êý×ÖÐ¡¼üÅÌÉÏµÄ 4 ¼ü
else if(string=="NUM5") return VK_NUMPAD5		;		//65		;		//Êý×ÖÐ¡¼üÅÌÉÏµÄ 5 ¼ü
else if(string=="NUM6") return VK_NUMPAD6		;		//66		;		//Êý×ÖÐ¡¼üÅÌÉÏµÄ 6 ¼ü
else if(string=="NUM7") return VK_NUMPAD7 		;		// 67		;		//Êý×ÖÐ¡¼üÅÌÉÏµÄ 7 ¼ü
else if(string=="NUM8") return VK_NUMPAD8 		;		// 68Êý×ÖÐ¡¼üÅÌÉÏµÄ 8 ¼ü
else if(string=="NUM9") return VK_NUMPAD9 		;		// 69 		;		// Êý×ÖÐ¡¼üÅÌÉÏµÄ 9 ¼ü
else if(string=="Multiply") return VK_MULTIPLY 		;		// 6A		;		//Multiply ¼ü
else if(string=="Add") return VK_ADD		;		//6B		;		//Add ¼ü
else if(string=="Separator") return VK_SEPARATOR		;		//6C		;		//Separator ¼ü
else if(string=="Subtract") return VK_SUBTRACT		;		//6D 		;		// Subtract ¼ü
else if(string=="Decimal") return VK_DECIMAL 		;		// 6E 		;		// Decimal ¼ü
else if(string=="Divide") return VK_DIVIDE 		;		// 6F 		;		// Divide ¼ü
else if(string=="F1") return VK_F1 		;		// 70 		;		// F1 ¼ü
else if(string=="F2") return VK_F2 		;		// 71 		;		// F2 ¼ü
else if(string=="F3") return VK_F3 		;		// 72 		;		// F3 ¼ü
else if(string=="F4") return VK_F4 		;		// 73 		;		// F4 ¼ü
else if(string=="F5") return VK_F5 		;		// 74 		;		// F5 ¼ü
else if(string=="F6") return VK_F6 		;		// 75 		;		// F6 ¼ü
else if(string=="F7") return VK_F7 		;		// 76 		;		// F7 ¼ü
else if(string=="F8") return VK_F8		;		//77 		;		// F8 ¼ü
else if(string=="F9") return VK_F9 		;		// 78 		;		// F9 ¼ü
else if(string=="10") return VK_F10 		;		// 79 		;		// F10 ¼ü
else if(string=="11") return VK_F11		;		//7A 		;		// F11 ¼ü
else if(string=="F12") return VK_F12		;		//7B 		;		// F12 ¼ü
else if(string=="F13") return VK_F13 		;		// 7C 		;		// F13 ¼ü
else if(string=="F14") return VK_F14		;		//7D		;		//F14 ¼ü
else if(string=="F15") return VK_F15		;		//7E		;		//F15 ¼ü
else if(string=="F16") return VK_F16		;		//7F		;		//F16 ¼ü
else if(string=="F17") return VK_F17		;		//80H		;		//F17 ¼ü
else if(string=="F18") return VK_F18		;		//81H		;		//F18 ¼ü
else if(string=="F19") return VK_F19		;		//82H		;		//F19 ¼ü
else if(string=="F20") return VK_F20 		;		// 83H		;		//F20 ¼ü
else if(string=="F21") return VK_F21		;		//84H		;		//F21 ¼ü
else if(string=="F22") return VK_F22		;		//85H		;		//F22 ¼ü
else if(string=="F23") return VK_F23		;		//86H		;		//F23 ¼ü
else if(string=="F24") return VK_F24		;		//87H		;		//F24 ¼ü
else if(string=="NUMLOCK") return VK_NUMLOCK 		;		// 90 		;		//		;		//UM LOCK ¼ü
else if(string=="SCROLL LOCK") return VK_SCROLL 		;		// 91 		;		// SCROLL LOCK ¼ü
//else if(string=="Att") return VK_ATT 		;		// F6		;		//Att ¼ü
else if(string=="CrSel") return VK_CRSEL		;		//F7		;		//CrSel ¼ü
else if(string=="ExSel") return VK_EXSEL		;		//F8		;		//ExSel ¼ü
else if(string=="Erase EOF") return VK_EREOF		;		//F9		;		//Erase EOF ¼ü
else if(string=="Play") return VK_PLAY		;		//FA Play 		;		//Play ¼ü
else if(string=="Zoom") return VK_ZOOM		;		//FB		;		//Zoom ¼ü
else if(string=="PA1") return VK_PA1 		;		// FD 		;		// PA1 ¼ü
else if(string=="Clear") return VK_OEM_CLEAR 		;		// FE 		;		// Clear ¼ü		;		//		;		//		;		//		;		// 
else return 0;
}
