#include <stdio.h>
#include <string.h>

#define DEF_LINE_LENGTH 1023

FILE *fp_builtinputfile = NULL;
char *builtinputfilename = "tmp_finalinputfiles.txt";


/**!Comment: This function concatenate the input stream 
 * Input: 
 * var_inputfile - name of the input file to concatenate 
 * OutPut: 
 * fp_builtinputfile - The final file with all input (global variable)
 * */
int func_vidbuildinputstream(char *var_inputfile){
   FILE *fp_inputfile = NULL;
   char char_buffer[DEF_LINE_LENGTH];

   fp_inputfile = fopen(var_inputfile, "r");
   if (fp_inputfile == NULL)
   {
      printf("errr %s",var_inputfile);
      return 1;
   } 
   /*!Comment: read fp_inputfile and append the content in fp_builtinputfile */
   while(fgets(char_buffer, DEF_LINE_LENGTH, fp_inputfile)){
      fprintf(fp_builtinputfile, "%s", char_buffer);
   }
   fprintf(fp_builtinputfile, "\n");

   /*!Comment: Free memory */
   fclose(fp_inputfile);
}


/**!Comment: This function reads input files for standard input
 * Input: standard Input
 * Output: array of file name
 **/
void func_vidrequestinputfiles(){
   char buf_localstr[DEF_LINE_LENGTH]; // temp buffer to hold string from standard input
   char loc_listOffile[DEF_LINE_LENGTH][DEF_LINE_LENGTH]; // temp buffer to hold the word (input file name)
   int i,j,filecount;
 
   printf("please enter the list of files : ");
   fgets(buf_localstr, sizeof buf_localstr, stdin);   

   /*!Comment: find word (files names) from string */
   j=0; filecount=0;
   for(i=0;i<=(strlen(buf_localstr));i++){
      if(buf_localstr[i]==' '|| buf_localstr[i]=='\0'){ // check the word ending
         loc_listOffile[filecount][j]='\0';
         filecount++; // Increment to word counter
         j=0;
      }
      else
      {
         loc_listOffile[filecount][j]=buf_localstr[i];
         j++;
      }
   }

   /*!Comment: Build the input stream*/
   for(i=0;i < filecount;i++){
      loc_listOffile[i][strcspn(loc_listOffile[i], "\n")] = 0; // Clean the word
      func_vidbuildinputstream(loc_listOffile[i]);
   }

   return;
}


int main(int argc, char **argv)
{
   int i;

   /*!Comment: get inputs */
   if (argc < 2) {
      printf("No dictfile found, the program will exit.\n");
      return 0;
   }
   fp_builtinputfile = fopen(builtinputfilename, "w");
   if (argc == 2){
      func_vidrequestinputfiles();
   }
   if (argc > 2){
      for (i = 2; i < argc; i++){
         func_vidbuildinputstream(argv[i]);
      }
   }
   fclose(fp_builtinputfile);


   return 0;
}


