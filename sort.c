#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//----------------------functions'_names-----------------------------------

int OpenFileForRead(FILE** file, char* file_name);

int OpenFileForWrite(FILE** file, char* file_name);

int FileLength(FILE* file);

int CountStr(FILE* file);

int PutBeginingsIndexes(char* buf, int length, char** beginings_of_str, int amount_of_str);

int Rotater(char* buf, int length, char** beginings_of_str, int amount_of_str);

int CreateNewBuf(char* buf, int length, char** beginings_of_str, int amount_of_str, char* bufnew);

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

    int length = FileLength(input_file);

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
    fread(buf, sizeof(char), length, input_file);
    fseek(input_file, 0, SEEK_SET);

    buf[length] = '\0';

    printf("str 73\n");

    int amount_of_str = CountStr(input_file);
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

    char* bufnew = (char*) calloc(length + 1, sizeof(char));

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

int FileLength(FILE* file) /*length without EOF*/
{
    if (file == NULL)
        return -1;

    //fseek(file, 0, SEEK_SET);

    int length = 1;

    while (getc(file) != EOF)
      length++;

    printf("length of file (expected without eof) = %d\n", length);
    fseek(file, 0, SEEK_SET);

    return length;
}

//------------------------------------------------------------------

int CountStr(FILE* file)
{
    if (file == NULL)
        return -1;

    fseek(file, 0, SEEK_SET);

    int count_str = 0;

    while (getc(file) != EOF)
    {
        fscanf(file, "%*[^\nEOF]");
        count_str++;
    }

    fseek(file, 0, SEEK_SET);

	return count_str;
}

//-----------------------------------------------------------------

int PutBeginingsIndexes(char* buf, int length, char** beginings_of_str, int amount_of_str) /*change \n on \0*/
{
    if (buf == NULL)
        return -1;

    if (beginings_of_str == NULL)
        return -1;

    int k = 1;

    beginings_of_str[0] = buf;

    for (int i = 1; i < length && k < amount_of_str; i++)
    {
        if (buf[i] == '\n')
        {
            beginings_of_str[k] = &buf[i + 1];
            buf[i] = '\0';
            k++;
        }
    }

    return 0;
}

//----------------------------------------------------------------

int Rotater(char* buf, int length, char** beginings_of_str, int amount_of_str)
{
    if (buf == NULL)
        return -1;

    if (beginings_of_str == NULL)
        return -1;

    char* rotate = 0;

    for (int j = 1; j < amount_of_str; j++)
    {
        for (int i = 1; i < amount_of_str; i++)
        {
            for (int k = 0; beginings_of_str[i] + k < &buf[length] && *(beginings_of_str[i - 1] + k) != '\0' && *(beginings_of_str[i] + k) != '\0'; k++)
            {
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

int CreateNewBuf(char* buf, int length, char** beginings_of_str, int amount_of_str, char* bufnew)
{
    if (buf == NULL)
        return -1;

    if (beginings_of_str == NULL)
        return -1;

    if (bufnew == NULL)
        return -1;

    int j = 0;

    for (int i = 0; i < amount_of_str; i++)
    {
        for (int k = 0; beginings_of_str[i] + k < &buf[length] && *(beginings_of_str[i] + k) != '\0'; k++)
        {
            bufnew[j] = *(beginings_of_str[i] + k);
            j++;
        }
        bufnew[j] = '\n';
        j++;
    }

    return 0;
}
