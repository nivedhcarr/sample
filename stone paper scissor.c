#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
int game(char you , char pc){
  int decision;
    if(you==pc)
        decision=-1;
    
    if(you=='s'&& pc=='p')
     decision=0;
 else if (you=='p'&& pc=='s')
  decision=1;
 if(you=='s'&& pc=='c')
   decision=1;
 else if(you=='c' && pc=='s')
  decision=0;
 if(you=='p' && pc=='c')
   decision=0;
 else if(you=='c' && pc=='p')
  decision=1;
 else if(you!='s' && you!='p' && you!='c'){
 
     printf("\ninvalid input\n");      
     decision=-2;}
 return decision;
}
int main(){
char you,pc;
int n,result;
srand(time(NULL));
n=rand()%3;
if(n==0)
 pc='s';
if(n==1)
 pc='p';
if(n==2)
 pc='c';
printf("\nplease choose one from s,p,c\n");
scanf(" %c",&you);
result=game(you,pc);
if(result==-1)
 printf("\noops ! you've drawn the game\n");
if(result==1)
 printf("\nyou won\n");
else if(result==0)
 printf("\nyou lost\n");
printf("\nWHAT JUST HAPPENED?\n");
printf("\nyou chose %c and pc chose %c\n",you,pc);
return 0;}

