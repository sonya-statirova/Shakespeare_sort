#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//----------------------functions'_names-----------------------------------

int OpenFileForRead(FILE** file, char* file_name);

int OpenFileForWrite(FILE** file, char* file_name);

unsigned long FileLength(FILE* file);

unsigned long CountStr(FILE* file);

int PutBeginingsIndexes(char* buf, unsigned long length, char** beginings_of_str, unsigned long amount_of_str);

int Rotater(char* buf, unsigned long length, char** beginings_of_str, unsigned long amount_of_str);

int CreateNewBuf(char* buf, unsigned long length, char** beginings_of_str, unsigned long amount_of_str, char* bufnew);

//------------------------main--------------------------------

int main()
{
    char input_file_name[60] = {};
    char output_file_name[60] = {};

    FILE* input_file = NULL;
    FILE* output_file = NULL;

    if (OpenFileForRead(&input_file, input_file_name) == -1)
    {
        printf("Cannot open input file\n");

        return -1;
    }

    if (OpenFileForWrite(&output_file, output_file_name) == -1)
    {
        printf("Cannot open output file\n");
        fclose(input_file);

        return -1;
    }

    unsigned long length = FileLength(input_file);

    if (length == -1)
    {
        printf("Incorrect file pointer\n");

        return -1;
    }

    printf("str 54\n");

    char* buf = (char*) calloc(length + 1, sizeof(char));

    if (buf == NULL)
    {
        printf("Cannot allocate memory for buf\n");
        return -1;
    }

    printf("str 65\n");

    fseek(input_file, 0, SEEK_SET);
    length = fread(buf, sizeof(char), length + 1, input_file);
    printf("file real length = %lu\n", length);
    fseek(input_file, 0, SEEK_SET);

    printf("str 73\n");

    unsigned long amount_of_str = CountStr(input_file);
    printf("number of str = %lu\n", amount_of_str);
    if (amount_of_str == -1)
    {
        printf("Incorrect file pointer\n");

        return -1;
    }

    printf("str 83\n");

    char** beginings_of_str = (char**) calloc(amount_of_str, sizeof(char*));

    if (beginings_of_str == NULL)
    {
        printf("Cannot allocate memory for beginings_of_str\n");
        fclose(input_file);
        fclose(output_file);
        free(buf);

        return -1;
    }

    printf("str 97\n");

    if (PutBeginingsIndexes(buf, length, beginings_of_str, amount_of_str) == -1)
    {
        printf("Incorrect array pointer\n");
        fclose(input_file);
        fclose(output_file);

        return -1;
    }

    printf("str 108\n");

    char* bufnew = (char*) calloc(length, sizeof(char));

    if (bufnew == NULL)
    {
        printf("Cannot allocate memory for bufnew\n");
        fclose(input_file);
        fclose(output_file);
        free(buf);
        free(beginings_of_str);

        return -1;
    }

    printf("str 123\n");

    if (Rotater(buf, length, beginings_of_str, amount_of_str) == -1)
    {
        printf("Incorrect array pointer\n");
        fclose(input_file);
        fclose(output_file);

        return -1;
    }

    printf("str 134\n");

    if (CreateNewBuf(buf, length, beginings_of_str, amount_of_str, bufnew) == -1)
    {
        printf("Incorrect array pointer\n");
        fclose(input_file);
        fclose(output_file);

        return -1;
    }

    printf("str 145\n");

    fseek(input_file, 0, SEEK_SET);
    fwrite(bufnew, sizeof(char), length, output_file);
    fseek(input_file, 0, SEEK_SET);

    fclose(input_file);
    fclose(output_file);
    free(buf);
    free(beginings_of_str);
    free(bufnew);

    printf("Your laba is ready and put into %s\n\n", output_file_name);

    return 0;
}

//----------------------functions-----------------------------------

int OpenFileForRead(FILE** file, char* file_name)
{
    printf("Please, enter adress of file for reading in format *.txt\n");
    scanf("%s", file_name);

    *file = fopen(file_name, "r");

    if (*file  == NULL)
        return -1;

    return 0;
}

//----------------------------------------------------------------

int OpenFileForWrite(FILE** file, char* file_name)
{
    printf("Please, enter adress of file for writing in format *.txt\n");
    scanf("%s", file_name);

    *file = fopen(file_name, "w");

    if (*file  == NULL)
        return -1;
    return 0;
}

//------------------------------------------------------------------

unsigned long FileLength(FILE* file) /*length without EOF*/
{
    if (file == NULL)
        return -1;

    unsigned long length = 0;

    while (getc(file) != EOF)
      length++;

  	/*fseek(file, 0, SEEK_END);

    length = (ftell(file) + 1);*/

    printf("length of file (expected without eof) = %lu\n", length);
    fseek(file, 0, SEEK_SET);

    return (length + 1);
}

//------------------------------------------------------------------

unsigned long CountStr(FILE* file)
{
    if (file == NULL)
        return -1;

    fseek(file, 0, SEEK_SET);

    unsigned long count_str = 0;

    while (getc(file) != EOF)
    {
        fscanf(file, "%*[^\nEOF]");
        count_str++;
    }

    fseek(file, -1, SEEK_END);

    if (getc(file) == '\n')
      count_str--;

    fseek(file, 0, SEEK_SET);

	return count_str;
}

//-----------------------------------------------------------------

int PutBeginingsIndexes(char* buf, unsigned long length, char** beginings_of_str, unsigned long amount_of_str) /*change \n on \0*/
{
    if (buf == NULL)
        return -1;

    if (beginings_of_str == NULL)
        return -1;

    beginings_of_str[0] = buf;

    unsigned long k = 1;
    unsigned long i = 0;

    for (i = 0; i < (length - 1) && k < amount_of_str; i++)
    {
        if (buf[i] == '\n')
        {
            beginings_of_str[k] = &buf[i + 1];
            buf[i] = '\0';
            k++;
        }
    }

    if (buf[i] == '\n')
      buf[i] = '\0';

    return 0;
}

//----------------------------------------------------------------

int Rotater(char* buf, unsigned long length, char** beginings_of_str, unsigned long amount_of_str)
{
  printf("am i alive?");
    if (buf == NULL)
        return -1;

    if (beginings_of_str == NULL)
        return -1;

    char* rotate = 0;

    for (unsigned long j = 1; j < amount_of_str; j++)
    {
        for (unsigned long i = 1; i < amount_of_str; i++)
        {
            for (unsigned long k = 0; beginings_of_str[i] + k < &buf[length] && beginings_of_str[i - 1] + k < &buf[length] && *(beginings_of_str[i - 1] + k) != '\0' && *(beginings_of_str[i] + k) != '\0'; k++)
            {
              assert(beginings_of_str[i] != NULL);
              assert(beginings_of_str[i - 1] != NULL);
              printf("%c", *(beginings_of_str[i] + k));
                if (*(beginings_of_str[i - 1] + k) < *(beginings_of_str[i] + k))
                {
                    break;
                }
                else if (*(beginings_of_str[i - 1] + k) > *(beginings_of_str[i] + k))
                {
                    rotate = beginings_of_str[i];
                    beginings_of_str[i] = beginings_of_str[i - 1];
                    beginings_of_str[i - 1] = rotate;
                    break;
                }
            }
        }
    }

    return 0;
}

//-----------------------------------------------------------------

int CreateNewBuf(char* buf, unsigned long length, char** beginings_of_str, unsigned long amount_of_str, char* bufnew)
{
    if (buf == NULL)
        return -1;

    if (beginings_of_str == NULL)
        return -1;

    if (bufnew == NULL)
        return -1;

    unsigned long j = 0;

    for (unsigned long i = 0; i < amount_of_str; i++)
    {
        for (unsigned long k = 0; beginings_of_str[i] + k < &buf[length] && *(beginings_of_str[i] + k) != '\0'; k++)
        {
            bufnew[j] = *(beginings_of_str[i] + k);
            j++;
        }
        bufnew[j] = '\n';
        j++;
    }

    return 0;
}
