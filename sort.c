#include <stdio.h>
#include <stdlib.h>
//----------------------functions'_names-----------------------------------

int InitializateAndOpenFileForRead(FILE** file, char* file_name);

int InitializateAndOpenFileForWrite(FILE** file, char* file_name);

unsigned long FileLength(FILE* file);

unsigned long CountStr(char* buf, unsigned long length);

int PutBeginingsIndexes(char* buf, unsigned long length, char** beginings_of_str, unsigned long amount_of_str);

int DeleteR_PutO(char* buf, unsigned long length);

int Comparer(unsigned long str1, unsigned long str2, char** beginings_of_str, char* buf);

int Sort(unsigned long amount_of_str, char** beginings_of_str, char* buf);

int CreateNewBuf(char* buf, unsigned long length, char** beginings_of_str, unsigned long amount_of_str, char* bufnew);

//------------------------main--------------------------------

int main()
{
    char input_file_name[60] = {};
    char output_file_name[60] = {};

    FILE* input_file = NULL;
    FILE* output_file = NULL;

    if (InitializateAndOpenFileForRead(&input_file, input_file_name) == -1)
            return -1;

    if (InitializateAndOpenFileForWrite(&output_file, output_file_name) == -1)
            return -1;

    unsigned long length = FileLength(input_file);

    if (length == -1)
            return -1;

    char* buf = (char*) calloc(length + 1, sizeof(char));

    if (buf == NULL)
        {
            printf("Cannot allocate memory for buf\n");
            fclose(input_file);
            fclose(output_file);

            return -1;
        }

    fseek(input_file, 0, SEEK_SET);
    fread(buf, sizeof(char), length, input_file);
    fseek(input_file, 0, SEEK_SET);

    unsigned long amount_of_str = CountStr(buf, length);

    if (amount_of_str == -1)
            return -1;

    char** beginings_of_str = (char**) calloc(amount_of_str, sizeof(char*));

    if (beginings_of_str == NULL)
        {
            printf("Cannot allocate memory for beginings_of_str\n");
            fclose(input_file);
            fclose(output_file);
            free(buf);

            return -1;
        }

    if (PutBeginingsIndexes(buf, length, beginings_of_str, amount_of_str) == -1)
            return -1;

    if (DeleteR_PutO( buf, length) == -1)
            return -1;

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

    if (Sort(amount_of_str, beginings_of_str, buf) == -1)
            return -1;

    if (CreateNewBuf(buf, length, beginings_of_str, amount_of_str, bufnew) == -1)
            return -1;

    fwrite(bufnew, sizeof(char), length, output_file);

    fclose(input_file);
    fclose(output_file);
    free(buf);
    free(beginings_of_str);
    free(bufnew);

    printf("File was sorted and put into %s\n\n", output_file_name);

    return 0;
}

//----------------------functions-----------------------------------

int InitializateAndOpenFileForRead(FILE** file, char* file_name)
{
    printf("Please, enter adress of file for reading in format *.txt\n");

    if (scanf("%s", file_name) != 1)
    {
      printf("error in InitializateAndOpenFileForRead\n");
      return -1;
    }

    *file = fopen(file_name, "r");

    if (*file  == NULL)
    {
        printf("Cannot open input file\n");
        return -1;
    }

    return 0;
}

//----------------------------------------------------------------

int InitializateAndOpenFileForWrite(FILE** file, char* file_name)
{
    printf("Please, enter adress of file for writing in format *.txt\n");

    if (scanf("%s", file_name) != 1)
    {
      printf("error in InitializateAndOpenFileForWrite\n");
      return -1;
    }

    *file = fopen(file_name, "w");

    if (*file  == NULL)
    {
        printf("Cannot open output file\n");
        return -1;
    }

    return 0;
}

//------------------------------------------------------------------

unsigned long FileLength(FILE* file) /*length without EOF*/
{
    if (file == NULL)
    {
      printf("error in FileLength\n");
      return -1;
    }

    unsigned long length = 0;

    while (getc(file) != EOF)
      length++;

    fseek(file, 0, SEEK_SET);

    return length;
}

//-------------------------------------------------------------------

unsigned long CountStr(char* buf, unsigned long length)
{
  if (buf == NULL)
  {
    printf("error in CountStr\n");
    return -1;
  }

	unsigned long count_str = 1;

	for (unsigned long i = 0; i < length - 1; i++)
	{
		if (buf[i] == '\n')
			count_str++;
	}

  return count_str;
}

//-----------------------------------------------------------------

int PutBeginingsIndexes(char* buf, unsigned long length, char** beginings_of_str, unsigned long amount_of_str)
{
    if (buf == NULL || beginings_of_str == NULL)
    {
      printf("error in PutBeginingsIndexes\n");
      return -1;
    }

    beginings_of_str[0] = buf;

    unsigned long k = 1;
    unsigned long i = 0;

    for (i = 0; i < (length - 1) && k < amount_of_str; i++)
    {
        if (buf[i] == '\n')
        {
            beginings_of_str[k] = &buf[i + 1];
            k++;
        }
    }

    return 0;
}

//------------------------------------------------------------------

int DeleteR_PutO(char* buf, unsigned long length)
{
  if (buf == NULL)
  {
    printf("error in DeleteR_PutO\n");
    return -1;
  }

  for (unsigned long i = 0; i < length; i++)
  {
      if (buf[i] == '\n')
          buf[i] = '\0';
  }

  return 0;
}

//----------------------------------------------------------------

int Comparer(unsigned long str1, unsigned long str2, char** beginings_of_str, char* buf)
{
  if (buf == NULL || beginings_of_str == NULL)
  {
    printf("error in Comparer\n");
    return -1;
  }

  unsigned long i1 = 0;
  unsigned long i2 = 0;
  char letter1 = '\0';
  char letter2 = '\0';

	while (*(beginings_of_str[str1] + i1) != '\0' && *(beginings_of_str[str2] + i2) != '\0')
	{
		letter1 = *(beginings_of_str[str1] + i1);
    letter2 = *(beginings_of_str[str2] + i2);

    if (!(('a' <= letter1 && letter1 <= 'z') || ('A' <= letter1 && letter1 <= 'Z')))
    {
      i1++;
      continue;
    }
    else if (!(('a' <= letter2 && letter2 <= 'z') || ('A' <= letter2 && letter2 <= 'Z')))
    {
      i2++;
      continue;
    }

    if ('A' <= letter1 && letter1 <= 'Z')
      letter1 = letter1 + 'a' - 'A';

    if ('A' <= letter2 && letter2 <= 'Z')
      letter2 = letter2 + 'a' - 'A';

    if (letter1 < letter2)
		  return 1; //str1 should be before str2
		else if (letter1 > letter2)
			return 2; //str1 should be after str2

    i1++;
    i2++;
	}

  if (*(beginings_of_str[str2] + i2) == '\0')
    return 2;
  else
    return 1;
}

//-----------------------------------------------------------------

int Sort(unsigned long amount_of_str, char** beginings_of_str, char* buf)
{
  if (buf == NULL || beginings_of_str == NULL)
  {
    printf("error in Sort\n");
    return -1;
  }

	unsigned long i = 0;
	char* k = 0;

  int compare_result = 0;

	for (unsigned long j = 0; j < amount_of_str - 1; j++)
	{
		i = 0;
		while (i < amount_of_str - 1)
		{
      compare_result = Comparer(i, i + 1, beginings_of_str, buf);
			if (compare_result == 2)
			{
				k = beginings_of_str[i];
				beginings_of_str[i] = beginings_of_str[i + 1];
				beginings_of_str[i + 1] = k;
			}
      else if (compare_result == -1)
        return -1;

			i++;
		}
	}

  return 0;
}

//-----------------------------------------------------------------

int CreateNewBuf(char* buf, unsigned long length, char** beginings_of_str, unsigned long amount_of_str, char* bufnew)
{
  if (buf == NULL || beginings_of_str == NULL || bufnew == NULL)
  {
    printf("error in CreateNewBuf\n");
    return -1;
  }

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
