#include<stdio.h>
#include<stdlib.h>
#include<error.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
  
#define DELI ";"
#define DELI2 " "
#define MAXCMDLEN 256
#define MAXNUMPRO 128
#define MAXOPTION 10

int main(int argc, char*argv[]){
	    char *f[MAXOPTION]; //instruction stroage by space
		char comd[MAXCMDLEN]={0}; //instruction input line
		char *piece=0; // instruction strage by ;
		pid_t pid[MAXNUMPRO];
		int status;
		int i=0;
		int j=0;
		FILE *fp;

		for(i=0;i<MAXOPTION;i++){
			f[i]=0;
		}
		i=0;

		if(argc<2){
			while(1){
			printf("prompt> ");
			fgets(comd,sizeof(comd),stdin);//because don't remain buffer
			comd[strlen(comd)-1]=';';// using when just one instruction
			if(strncmp(comd,"quit",4)==0|| feof(stdin)==1){
				return 0;
			}else{
				j=0;
				i=0;
				piece=strtok(comd,DELI);//divide by ;
				while(piece!=NULL){
					if((pid[i]=fork())<0){					
						fprintf(stderr,"fork error\n");
						exit(-1);
					}else if(pid[i]==0){//child process		
						f[j]=strtok(piece,DELI2);						
						while(f[j]!=NULL){//띄어쓰기로 받은 명령어 분할
							f[++j]=strtok(NULL,DELI2);
						}						
						if(execvp(f[0],f)==-1){
							fprintf(stderr,"exec error or Not command %s\n",f[0]);
							exit(-1);
						}//execute corresponding instruction with argument
						exit(1);//quit child process
					}					
					piece=strtok(NULL,DELI);//divide by ; 
					i++;
				}}
			for(j=0;j<i;j++){
				waitpid(pid[j],&status,WUNTRACED);
				if(WIFEXITED(status)==0){
					fprintf(stderr,"child process finish error\n");
					exit(-1);
				}
			}
		}
		}else{
			if((fp = fopen(argv[1],"r"))==NULL){
				fprintf(stderr,"No open file or No exist file \n");
				exit(-1);
			}
			while(1){
				if(fgets(comd,sizeof(comd),fp)==NULL){//get one line
					fclose(fp);
					return 0;
				}
				printf("%s",comd);
				comd[strlen(comd)-1]=';';// using when just one instruction
				if(strncmp(comd,"quit",4)==0){
					fclose(fp);
					return 0;
				}else{
					i=0;
					j=0;
					piece=strtok(comd,DELI);//divide by ;
					while(piece!=NULL){
						if((pid[i]=fork())<0){					
							fprintf(stderr,"fork error\n");
							fclose(fp);
							exit(-1);
						}else if(pid[i]==0){//child process		
							f[j]=strtok(piece,DELI2);						
							while(f[j]!=NULL){//띄어쓰기로 받은 명령어 분할
								f[++j]=strtok(NULL,DELI2);
							}						
							if(execvp(f[0],f)==-1){
								fprintf(stderr,"exec error or No command\n");
								exit(-1);
							}//execute corresponding instruction with argument
							exit(1);//quit child process
						}					
						piece=strtok(NULL,DELI);//divide by ; 
						i++;
					}
				}
				for(j=0;j<i;j++){
					waitpid(pid[j],&status,WUNTRACED);
					if(WIFEXITED(status)==0){
						fprintf(stderr,"child process finish error\n");
						fclose(fp);
						exit(-1);
					}
				}
			}
		}
}

