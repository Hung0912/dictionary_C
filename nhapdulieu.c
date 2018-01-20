#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"btree.h"
#include"soundex.h"

#define max 80
#define maxn 1000000

int TimKitu(char c, char *s)
{
int i;
  for (i=0;i<strlen(s);i++)
    if (c==s[i]) return i;
}
//Doi ma 64 lay vi tri va do dai tu trong file dict
int GetDemicalValue(char *s)
{
  char base64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  int i,decValue=0,pos,n=strlen(s),p;
  for (i=0;i<n;i++)
    {
      pos=TimKitu(s[i],base64);
      p=(int)pow(64,n-i-1);
      decValue+=pos*p;
    }
  return decValue;
}

int Nhapdulieu(BTA *dict, BTA *soundex, FILE *findex, FILE *fdict)
{
  int i,j,n,realsize;
  int vitri,dodai;
  char buff[max],key[max],vitri_nghia[max],dodai_nghia[max],soundex_key[max];
  //Nhap du lieu vao cay Btree
  while (fgets(buff,max,findex)!=NULL)
    {
      i=0; j=0;
      
      while (buff[i]!='\t') 
	{
	  key[i]=buff[i];
	  i++;
	}
      key[i]='\0';
      i++; j=0;
      while (buff[i]!='\t')
	{
	  vitri_nghia[j]=buff[i];
	  i++; j++;
	}
      vitri_nghia[j]='\0';
      i++; j=0;
      while (buff[i]!='\n')
	{
	  dodai_nghia[j]=buff[i];
	  i++; j++;
	}
      dodai_nghia[j]='\0';
      //printf("%s\n%s %s\n",key,vitri_nghia,dodai_nghia);
      //Lay nghia
      vitri=GetDemicalValue(vitri_nghia);
      dodai=GetDemicalValue(dodai_nghia);
      //printf("%d %d\n",vitri,dodai);
      char *mean=(char*)malloc(sizeof(char)*(dodai+1));
      fseek(fdict,vitri,0);
      for (i=0;i<dodai;i++)
	mean[i]=fgetc(fdict);
      mean[i]='\0';
      //Co 1 so tu viet hoa
      for (i=0;i<strlen(key);i++)
	if (key[i]>'9'&&key[i]<'a')
	  key[i]=tolower(key[i]);
      
      //printf("%s\nMean:\n%s\n",key,mean);
      n=btins(dict,key,mean,(dodai+1)*sizeof(char));
      if (n!=0)
	{
	  printf("Khong nhap duoc du lieu!\n");
	  return 1;
	}
      //Nhap du lieu vao cay soundex
      strcpy(buff,"");
      SoundEx(soundex_key,key,0,0);
      strcat(key,"\n");
      //printf("%s %s\n",soundex_key,key);
      if (btsel(soundex,soundex_key,buff,maxn,&realsize))
	{
	  n=btins(soundex,soundex_key,key,(strlen(key)+1)*sizeof(char));
	  if (n!=0)
	    {
	      printf("Khong nhap duoc du lieu!\n");
	      return 1;
	    }
	}
      else btupd(soundex,soundex_key,key,(strlen(key)+1)*sizeof(char));
      //printf("%s",buff);
}
}

int main()
{
BTA *dict=NULL,*soundex=NULL; 
FILE *findex,*fdict;
int m,n;
//Mo 2 file CSDL
  if ((findex=fopen("anhviet.index","r+"))==NULL)
    {
      printf("Khong mo duoc file index!\n");
      return 1;
    }
  if ((fdict=fopen("anhviet.dict","r+"))==NULL)
    {
      printf("Khong mo duoc file dict!\n");
      return 1;
    }
  if (!soundex)
    {
      soundex=btcrt("soundex.dat",0,0);
      //m=btdups(soundex,TRUE); //Enable duplicate keys
    }
  if (!dict)
    {
      //printf("Tao tu dien\n");
      dict=btcrt("dictionary.dat",0,0);
      m=btdups(dict,TRUE); //Enable duplicate keys
      
      	Nhapdulieu(dict,soundex,findex,fdict);
	printf("Nhap du lieu thanh cong!\n");
    }
	  btcls(dict);
	  btcls(soundex);
fclose(findex);
fclose(fdict);
return 0;
}
