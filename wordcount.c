#include <stdio.h>
#include <string.h>

#define DEF_LINE_LENGTH 1023


/*!Comment: Global variables used inthe program*/
int VAR_TOTALWORDCOUNT = 0; // Total number of word in the file
FILE *FP_BUILTINPUTFILE = NULL; // For concatenation of inputs files
FILE *FP_DICTFILESTREAM = NULL; // For dictfile after parsing
char *VAR_BUILTINPUTFILENAME = "tmp_finalinputfiles.txt";
char *VAR_BUILTDICTFILENAME = "tmp_finaldictfile.txt";
int VAR_ERROR = 0;
int VAR_FUNCNUMBER = 99;


/*!Comment: This function deletes temp files 
 * Input: files name to delate: VAR_BUILTINPUTFILENAME, VAR_BUILTDICTFILENAME
 * */
void func_cleanrepo(){
   if ((remove(VAR_BUILTINPUTFILENAME) == 0) && (remove(VAR_BUILTDICTFILENAME) == 0)) {
       /*!Comment: do nothing*/
   } else {
      VAR_FUNCNUMBER = 1;
      VAR_ERROR = 1;
   }
}

/*!Comment: This function counts the number of words and display the result 
 * Input: 
 * VAR_BUILTDICTFILENAME - file with dictfile words (keys)
 * VAR_BUILTINPUTFILENAME - file with all inputfile concatenated
 * Output:
 * Result of count 
 * */
void func_vidcountwordfromdicttoinput(){
   char loc_currword[DEF_LINE_LENGTH];
   FILE *fp_finaldictfile = NULL;
   FILE *fp_finalinputfile = NULL;
   int value = 1;
   int loc_incr;
   char key[1023];  // Read from dictfile
   char pattern[1023];  // Read from input file
   int flag = 0; // To disable the counter once done

   fp_finalinputfile = fopen(VAR_BUILTDICTFILENAME, "r");
   if (fp_finalinputfile == NULL){
      VAR_FUNCNUMBER = 2;
      VAR_ERROR = 1;
   } else {
      /*!Comment: parse through the two file and compare entries */
      while ((fscanf(fp_finalinputfile, " %1023s", pattern) == 1) && (VAR_ERROR == 0)){
         pattern[strcspn(pattern, "\n")] = 0;
         fp_finaldictfile = fopen(VAR_BUILTINPUTFILENAME, "r");
         if (fp_finaldictfile == NULL){
            VAR_FUNCNUMBER = 2;
            VAR_ERROR = 1;
         }
         loc_incr = 0;
         while (fscanf(fp_finaldictfile, " %1023s", key) == 1){
           key[strcspn(key, "\n")] = 0;
            value = strcmp(key,pattern); // Compare the current word
            // Increment the word counter if the words are equal
            if(value == 0){
               loc_incr ++; 
            }      
            if (flag == 0){
               VAR_TOTALWORDCOUNT ++;
            }
         }   
         flag = 1;
         fclose(fp_finaldictfile);
   
         /*!Comment: provide results */
         if(loc_incr > 0){
            printf("%d\t%s\n", loc_incr, pattern);
         }
   
      }
   }
   fclose(fp_finalinputfile);
   return;
}


/*!Comment: This function parses dictfile and removes empty lines and # started lines.
 * Input: var_dictfilename - dictfile file name (from commad line)
 * Output: VAR_BUILTDICTFILENAME - file where key from dictfile are saved 
 * */
int func_vidParseDictfile(char *var_dictfilename){
   char char_buffer[DEF_LINE_LENGTH];
   char loc_StringOfWords[DEF_LINE_LENGTH][DEF_LINE_LENGTH];
   int loc_wordcount=0; // Index to count the number of word
   int i; //index to run through characters
   int j; // index to run through the words
   FILE *fp_wordtocountstream = NULL;


   FP_DICTFILESTREAM = fopen(var_dictfilename, "r");
   fp_wordtocountstream = fopen(VAR_BUILTDICTFILENAME, "w");
   if ((FP_DICTFILESTREAM == NULL) || (fp_wordtocountstream == NULL)){
      VAR_FUNCNUMBER = 3;
      VAR_ERROR = 1;
   }
   else {
      while(fgets(char_buffer, DEF_LINE_LENGTH, FP_DICTFILESTREAM)){
         if((!(strlen(char_buffer) == 0)) && (char_buffer[0] != '#') && (char_buffer[0] != '\n')){
            j=0; 
            loc_wordcount=0;
            for(i=0;i<=(strlen(char_buffer));i++)
            {
               /*!Comment: find word from new line */
               if(char_buffer[i]==' '||char_buffer[i]=='\0')
               {
                  loc_StringOfWords[loc_wordcount][j]='\0';
                  loc_wordcount++;
                  j=0;
              }
               else
               {
                  loc_StringOfWords[loc_wordcount][j]=char_buffer[i];
                  j++;
               }
            }
   
            /*!Comment: build the new dictfile and create an input stream from one word line of dictfile */
           if (loc_wordcount == 1){
               fprintf(fp_wordtocountstream, "%s", loc_StringOfWords[0]);
            }
       }
      }
   
      /*!Comment: Free memory */
      fclose(FP_DICTFILESTREAM);
      fclose(fp_wordtocountstream);
   } 
}


/**!Comment: This function concatenate the input stream 
 * Input: 
 * var_inputfile - name of the input file to concatenate 
 * OutPut: 
 * FP_BUILTINPUTFILE - The final inputs file concatenated with all inputs (global variable)
 * */
int func_vidbuildinputstream(char *var_inputfile){
   FILE *fp_inputfile = NULL;
   char char_buffer[DEF_LINE_LENGTH];

   fp_inputfile = fopen(var_inputfile, "r");
   if (fp_inputfile == NULL)
   {
      VAR_FUNCNUMBER = 4;
      VAR_ERROR = 1;
   } 
   else {
      /*!Comment: read fp_inputfile and append the content in FP_BUILTINPUTFILE */
      while(fgets(char_buffer, DEF_LINE_LENGTH, fp_inputfile)){
         fprintf(FP_BUILTINPUTFILE, "%s", char_buffer);
      }
      fprintf(FP_BUILTINPUTFILE, "\n");

      /*!Comment: Free memory */
      fclose(fp_inputfile);
   }
}


/**!Comment: This function reads input files for standard input
 * Input: standard Input
 * Output: array of file name read from standard input
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
         loc_listOffile[filecount][j]=buf_localstr[i]; // save the charaters in the word
         j++; // Run through charaters
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
   FP_BUILTINPUTFILE = fopen(VAR_BUILTINPUTFILENAME, "w");
   if (FP_BUILTINPUTFILE == NULL)
   {
      VAR_FUNCNUMBER = 0;
      VAR_ERROR = 1;
   } else {
      if (argc == 2){
         func_vidrequestinputfiles();
      }
      if (argc > 2){
         for (i = 2; i < argc; i++){
            func_vidbuildinputstream(argv[i]);
         }
      }
      func_vidParseDictfile(argv[1]);
      fclose(FP_BUILTINPUTFILE);
   }

   if (VAR_ERROR == 0){
      func_vidcountwordfromdicttoinput();
   }

   if (VAR_ERROR == 0){
      func_cleanrepo();
   }

   /*!Comment: provide results */
   if (VAR_ERROR == 0){
      printf("%d total words\n", VAR_TOTALWORDCOUNT);
   }

   if (VAR_ERROR == 1){
      printf("Error found opening a file in function number : %d\n", VAR_FUNCNUMBER);
      return 1;
   } else {
      return 0;
   }
}


