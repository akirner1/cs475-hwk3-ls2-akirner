#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


/**
 * example documentation comment
 */


char* elementToString(struct stat *fileinfo, char* fileName, int isDir, char* prefix){
    int fileSize = fileinfo->st_size;
    char* parsedFile;
    if(isDir != 0){
        parsedFile = malloc(sizeof("/ (directory)") + strlen(fileName) + strlen(prefix));
        sprintf(parsedFile, "%s%s/ (directory)", prefix, fileName);
    }else{
        //integer.MAX_VALUE ~ 2.17billion, which is 1 characters long + 1 for the sign (unsure if c has signed ints, but worse case scenario we waste some extra memoery)
        parsedFile = malloc(sizeof(" (") + sizeof(char)*11 + sizeof(" bytes") + strlen(fileName) + strlen(prefix));
        sprintf(parsedFile, "%s%s (%d bytes)", prefix, fileName, fileSize);
    }
    //printf("%s",parsedFile);
    return parsedFile;
    
}
/**
 * @param pathName the name of a path (based of where the code is executed from) to the directory
 * @param exactPatternMatch filters elements to only ones whose children have 
 */
int collectElements(char *pathName, char *exactPatternMatch, int levelsDeep, struct stack_t* toPrint){
    char* prefix = (char*) malloc(levelsDeep*4 + 1);
    prefix[0] = 0;
    for(int i = 0; i < levelsDeep; i++){
        strcat(prefix, "    ");
    }
    //printf("entering level %d: %s\n", levelsDeep, pathName);
    //printf("%ld\n", strlen(pathName));
    //char* constPathName = (char*) malloc(strlen(pathName) + 1);
    //strcpy(constPathName, pathName);
    DIR *currentDir = opendir(pathName);
    int containsMatchingElements = 0;
    if (currentDir == NULL){
        //Something went wrong while opening the directory.
        //This is most likely either due to bad permissions or the element not being a directory
        //By returning, we are treating it the same was as if there were no matching strings inside the directory, so nothing inside will be printed
        return containsMatchingElements;
    }

    struct dirent *pDirent = 0;
    //char* dirName = (char*)malloc(sizeof(pDirent->d_name));
    //char dirName[sizeof(pDirent->d_name)]; 
    struct stat *fileinfo;
    char* fullPath;
    while((pDirent = readdir(currentDir))){
        fileinfo = (struct stat*) malloc(sizeof(struct stat));
        char* dirName = pDirent->d_name;
        fullPath = malloc(strlen(pathName) + 1 + strlen(dirName) + 1);
        sprintf(fullPath, "%s/%s", pathName, dirName);
        //printf("%s ---- ", fullPath);
        lstat(fullPath, fileinfo);
        if(S_ISDIR(fileinfo->st_mode)){
            if(strcmp(dirName, ".") != 0 && strcmp(dirName, "..") != 0){
                //printf("descending into %s", fullPath);
                int matchingElements = collectElements(fullPath, exactPatternMatch, levelsDeep + 1, toPrint);
                if(matchingElements > 0 ){
                    char* dirString = elementToString(fileinfo, dirName, 1, prefix);
                    //printf("%s\n", dirString);
                    push(toPrint, dirString);
                    //free(dirString);
                    //printf("\n");
                    containsMatchingElements += matchingElements;
                }else if(strstr(dirName, exactPatternMatch) != NULL){
                    char* dirString = elementToString(fileinfo,dirName, 1, prefix);
                    //printf("%s\n", dirString);
                    push(toPrint, dirString);
                    //free(dirString);
                    //printf("\n");
                }
            }
        }else{
            if(strstr(dirName, exactPatternMatch)){
                char* fileString = elementToString(fileinfo, dirName, 0, prefix);
                //printf("%s\n", fileString);
                push(toPrint, fileString);
                //free(fileString);
                //printf("\n");
                containsMatchingElements += 1;
            }
        }
        free(fullPath);
        free(fileinfo);
    }
    
    

    return containsMatchingElements;


}

/**
 * @param defaultFilePath[] a string containing the file path you would like to start with, 
 * relative to where the code was executed
 * @param exactPatternMatch[] a string to filter the listed files, or an empty string to not filter
 * 
 * @returns a stack where each entry is a string. These strings are ordered in such a way that printstack() will 
 * print the desired result of a recursive ls, with only files adhering to the exactPatternMatch and their paths being
 * printed, along with each element having (directory) or (n bytes) shown after the name for further representations
 */
struct stack_t* constructLsStack(char *defaultFilePath, char *exactPatternMatch){
    struct stack_t* toPrint = initstack();
    collectElements(defaultFilePath, exactPatternMatch, 0, toPrint);
    return toPrint;
}