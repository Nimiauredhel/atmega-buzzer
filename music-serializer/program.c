#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../music/toserialize.c"

int main(int argc, char** argv)
{
    char *outputPath = malloc(sizeof(char)*80);
    strcpy(outputPath, "./output.bin");

    FILE *outFile = fopen(outputPath, "w");
    size_t written =
    fwrite(voiceOne, sizeof(voiceOne[0]), voiceOneLength, outFile);
    printf("Number of elements written: %zu\n", written);
    fclose(outFile);

    char *newCharArr = malloc(sizeof(voiceOne));
    FILE *inFile = fopen(outputPath, "r");
    size_t read =
    fread(newCharArr, sizeof(voiceOne[0]), voiceOneLength, inFile); 
    printf("Number of elements read: %zu\n", read);

    printf("Read file elements:\n");

    for (int i = 0; i < voiceOneLength; i++)
    {
        printf("%u\n", newCharArr[i]);
    }

    printf("Program finished. Thank you.\n");

}
