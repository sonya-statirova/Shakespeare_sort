#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//----------------------functions'_names-----------------------------------

int OpenFileForRead(FILE** file, char* file_name);

int OpenFileForWrite(FILE** file, char* file_name);

int FileLength(FILE* file);

int CountStr(FILE* file);

void PutBeginingsIndexes(char* buf, int length, char** beginings_of_str);

void Rotater(char* buf, char** beginings_of_str, int amount_of_str);

void CreateNewBuf(char* buf, char** beginings_of_str, int amount_of_str, char* bufnew);

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

    printf("%d\n",input_file);

    if (OpenFileForWrite(&output_file, output_file_name) == -1)
    {
        printf("Cannot open output file\n");
        return -1;
    }

    int length = FileLength(input_file);

    if (length == -1)
    {
        printf("Incorrect file pointer\n");
        return -1;
    }

    char* buf = (char*) calloc(length + 1, sizeof(char));


    if (buf == NULL)
    {
        printf("Cannot allocate memory for buf\n");
        return -1;
    }

    printf("{%d}", fread(buf, 1, length, input_file)); //troubles with pointer of course
    buf[length + 1] = '\0';

    printf("%c%c%c%c", buf[0], buf[1], buf[2], buf[3]);

    int amount_of_str = CountStr(input_file);
    char** beginings_of_str = (char**) calloc(amount_of_str, sizeof(char*));

    if (beginings_of_str == NULL)
    {
        printf("Cannot allocate memory for beginings_of_str\n");
        return -1;
    }

    PutBeginingsIndexes(buf, length, beginings_of_str);

    char* bufnew = (char*) calloc(length + 1, sizeof(char));

    if (bufnew == NULL)
    {
        printf("Cannot allocate memory for bufnew\n");
        return -1;
    }

    Rotater(buf, beginings_of_str, amount_of_str);

    CreateNewBuf(buf, beginings_of_str, amount_of_str, bufnew);

    fwrite(bufnew, sizeof(char), length - 1, output_file);

    fclose(input_file);
    fclose(output_file);
    //free(buf);
    //free(bufnew);

    return 0;
}

//----------------------functions-----------------------------------

int OpenFileForRead(FILE** file, char* file_name)
{
    printf("Please, enter adress of file for reading in format *.txt\n");
    scanf("%s", file_name);

    *file = fopen(file_name, "rt");

    printf("%d\n", *file);

    if (*file  == NULL)
        return -1;

    return 0;
}

//----------------------------------------------------------------

int OpenFileForWrite(FILE** file, char* file_name)
{
    printf("Please, enter adress of file for writing in format *.txt\n");
    scanf("%s", file_name);

    *file = fopen(file_name, "wt");

    if (*file  == NULL)
        return -1;

    return 0;
}

//------------------------------------------------------------------

int FileLength(FILE* file)
{
    if (file  == NULL)
        return -1;

    fseek(file, 0, SEEK_SET);

    int length = 1;
    char current_char = getc(file);

    for (int i = 0; getc(file) != EOF; i++)
        length++;

    /*fseek(file, 0, SEEK_END);
    int length = ftell(file) + 1;
    fseek(file, 0, SEEK_SET);*/

    printf("%d", length);

    return length;
}

//------------------------------------------------------------------

int CountStr(FILE* file)
{
    fseek(file, 0, SEEK_SET);

    int count_str = 0;

    while (getc(file) != EOF)
    {
        fscanf(file, "%*[^\nEOF]");
        count_str++;
    }

    fseek(file, 0, SEEK_SET);

    assert(count_str == 3);

	return count_str;
}

//-----------------------------------------------------------------

void PutBeginingsIndexes(char* buf, int length, char** beginings_of_str)
{
    int k = 1;

    beginings_of_str[0] = buf;

    for (int i = 1; i < length; i++)
    {
        if (buf[i] == '\n')
        {
            beginings_of_str[k] = &buf[i + 1];
            k++;
        }
    }
    assert(*beginings_of_str[0] == 'a');
    assert(*beginings_of_str[1] == 'd');
    assert(*beginings_of_str[2] == 'b');
    assert(*beginings_of_str[3] == '\0');
}

//----------------------------------------------------------------

void Rotater(char* buf, char** beginings_of_str, int amount_of_str)
{
    char* rotate = 0;// NULL

    for (int i = 1; i < amount_of_str; i++)
    {
        for (int k = 0; *(beginings_of_str[i + k]) != '\n' && *(beginings_of_str[i - 1 + k]) != '\n'; k++)
        {
            if (*(beginings_of_str[i + k]) < *(beginings_of_str[i - 1 + k]))
            {
                rotate = beginings_of_str[i];
                beginings_of_str[i] = beginings_of_str[i - 1];
                beginings_of_str[i - 1] = rotate;
                break;
            }
        }
    }
}

//-----------------------------------------------------------------

void CreateNewBuf(char* buf, char** beginings_of_str, int amount_of_str, char* bufnew)
{
    int j = 0;

    for (int i = 0; i < amount_of_str; i++)
    {
        for (int k = 0; *(beginings_of_str[i + k]) != '\n'; k++)
        {
            bufnew[j] = *(beginings_of_str[i + k]);
            j++;
        }
    }
}
