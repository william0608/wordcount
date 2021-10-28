#include <stdio.h>
#include <string.h>

#define DEF_LINE_LENGTH 65



/*! This function reads input files for standard input*/
int func_vidrequestinputfiles(){
   char buf_localstr[DEF_LINE_LENGTH];
   char loc_listOffile[DEF_LINE_LENGTH][DEF_LINE_LENGTH]; 
   int i,j,filecount;
 
   printf("please enter the list of files : ");
   fgets(buf_localstr, sizeof buf_localstr, stdin);   

   j=0; filecount=0;
   for(i=0;i<=(strlen(buf_localstr));i++){
      if(buf_localstr[i]==' '|| buf_localstr[i]=='\0'){
         loc_listOffile[filecount][j]='\0';
         filecount++;
         j=0;
      }
      else
      {
         loc_listOffile[filecount][j]=buf_localstr[i];
         j++;
      }
    }

    // Build the input file
    for(i=0;i < filecount;i++){
      loc_listOffile[i][strcspn(loc_listOffile[i], "\n")] = 0;
      printf("%s - ",loc_listOffile[i]); // TODO check the last element
    }

}


int main(int argc, char **argv)
{
   int i;

   /*!Comment: get inputs */
   if (argc < 2) {
      printf("No dictfile found, the program will exit.\n");
      return 0;
   }
   if (argc == 2){
      func_vidrequestinputfiles();
   }
   if (argc > 2){
   }


   return 0;
}


