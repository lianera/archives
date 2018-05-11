/**********************************************
 Copyright 2012 lianera 
 Author:Lianera
 E-mail:158157488@qq.com
 Blog:www.lianera.com

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********************************************/

#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h>
#include <conio.h>
#include <winsock.h> 
#pragma comment(lib, "ws2_32.lib") 

#define CLRIN fflush(stdin);
#define MAXPATH 100
#define MAXNAME 100
#define MAXFORMAT 100
#define MAXHEAD 300
#define MAXPAGEBUF 100*1024
#define TASKFILE "cfg.dat"
#define LISTFILE task->name
#define FINDSTR "username=\""
#define ENDFLAG '\"'
#define TIEBAHOST "tieba.baidu.com"
#define PATH "/f/like/manage/list?kw="
#define HEADER "GET %s HTTP/1.1\r\nHOST: %s\r\nConnection: Close\r\n\r\n"

typedef struct tagTASK
{
	char path[MAXPATH],format[MAXFORMAT],name[MAXNAME];
	long start,prst,end,usrnum;

}TASK;

int readtask(TASK *task);
int addtask(TASK *task);
int explore(TASK *task);
int finduser(char* buf,char format[],FILE* fp);

int main()
{
	TASK task;
	puts("New Project?(y/n)");
	if(getch()=='y')
		addtask(&task);
	else if(!readtask(&task)&&!addtask(&task))
		{
			puts("The program has to exit");
			getch();
			exit(0);
		}
		
	explore(&task);
	getch();
	return 0;
}

int readtask(TASK *task)
{
	FILE* fp;
	if((fp=fopen(TASKFILE,"rb"))==NULL)
		return 0;
	rewind(fp);
	if(!fread(task,sizeof(TASK),1,fp))
		return 0;
	fclose(fp);
	return 1;
}

int addtask(TASK *task)
{
	FILE* fp;
	TASK tmptsk;

	do{puts("Tieba Name:");CLRIN;gets(tmptsk.name);}while(*tmptsk.name=='\0');
	do
	{
		puts("Start page,be sure page>=1:");
		CLRIN;
		scanf("%ld",&tmptsk.start);
	}while(tmptsk.start<1);
	tmptsk.prst=tmptsk.start;
	do
	{
		printf("End page,be sure page>=%ld,if all wanted, make it big enough:\n",tmptsk.start);
		CLRIN;
		scanf("%ld",&tmptsk.end);
	}while(tmptsk.end<tmptsk.start);
	do
	{
		puts("List Format,like this: \"Welcome:@%s \"");
		puts("%s\" is the flag to be filled with username:");
		CLRIN;
		gets(tmptsk.format);
	}while(*tmptsk.format=='\0');
	tmptsk.usrnum=0;
	if((fp=fopen(TASKFILE,"wb"))==NULL)
	{
		puts("Can not open task file!");
		return 0; 
	}
	
	if(!fwrite(&tmptsk,sizeof(TASK),1,fp))
	{
		fclose(fp);
		return 0;
	}
	memcpy(task,&tmptsk,sizeof(TASK));
	fclose(fp);
	remove(LISTFILE);
	return 1;
}

int getpath(TASK *task)
{
	char *nameutf=task->path;
	static char hexch[]="0123456789ABCDEF";
	char numstr[20];
	int i,j;
	strcpy(nameutf,PATH);
	for(i=strlen(nameutf),j=0;task->name[j]!='\0';i++,j++)
	{
		nameutf[i++]='%';
		nameutf[i++]=hexch[(unsigned char)task->name[j]/16];
		nameutf[i]=hexch[(unsigned char)task->name[j]%16];
	}
	nameutf[i]='\0';
	sprintf(numstr,"&pn=%d",task->prst+1);
	strcat(nameutf,numstr);
	return 1;
}

int explore(TASK *task)
{ 
	WSADATA WSAData={0}; 
	SOCKET sockfd; 
	struct sockaddr_in addr; 
	struct hostent *pURL; 
	char *pHost = 0, *pGET = 0;
	static char header[MAXHEAD] ={0}; 
	static char pagebuf[MAXPAGEBUF];
	char *pbufrcv;
	FILE* fp,*fpw;
	long rcvlen,tmp;

	if((fp=fopen(LISTFILE,"at"))==NULL)
		{puts("Can not open userlist file!");return 0;}
	if((fpw=fopen(TASKFILE,"wb+"))==NULL)
		{puts("Can not open config file!");return 0;}
	if(WSAStartup(MAKEWORD(2,2), &WSAData)) 
		{puts("WSA failed");return 0;} 
	
	pURL = gethostbyname(TIEBAHOST); 
	addr.sin_family = AF_INET; 
	addr.sin_addr.s_addr = *((unsigned long*)pURL->h_addr); 
	addr.sin_port = htons(80); 

	for(;task->prst<=task->end;task->prst++)
	{
		//Output Information
		system("cls");
		printf("Tieba Name:\t%s\n",task->name);
		printf("Page:   now:\t%ld from %ld to %ld \n",task->prst,task->start,task->end);
		printf("User number:\t%ld\n",task->usrnum);
		printf("Save format:\t");
		printf(task->format,"USERNAME");
		puts("\n[Press 'q' to exit,'p' to pause]\nReading...");

		getpath(task);
		sprintf(header,HEADER,task->path,TIEBAHOST); 
		pbufrcv=pagebuf;

		//Connect
		sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
		connect(sockfd,(SOCKADDR *)&addr,sizeof(addr)); 
		send(sockfd, header, strlen(header), 0); 
		while ((rcvlen=recv(sockfd,pbufrcv,BUFSIZ,0))> 0) 
			pbufrcv+=rcvlen;
		closesocket(sockfd); 
		
		*pbufrcv='\0';
		if((tmp=finduser(pagebuf,task->format,fp))<=0) break;
		task->usrnum+=tmp;
		rewind(fpw);
		fwrite(task,sizeof(TASK),1,fpw);
		if(kbhit())
		{
			if((tmp=getch())=='q')break;
			else if(tmp=='p') {puts("Paused...\nPress any key to go on");getch();}
		}
	}
	puts("Done!");
	WSACleanup();
	fclose(fpw);
	fclose(fp);
	return 1;
} 

int finduser(char* buf,char format[],FILE* fp)
{
	char *pt=buf,*efp,user[50]="";
	int usernum=0;
	while((pt=strstr(pt,FINDSTR))!=NULL)
	{
		pt+=strlen(FINDSTR);
		if(!(efp=strchr(pt,ENDFLAG)))break;
		*efp='\0';
		fprintf(fp,format,pt);
		*efp=ENDFLAG;
		usernum++;
	}
	return usernum;
}