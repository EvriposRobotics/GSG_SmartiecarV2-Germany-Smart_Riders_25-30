int Picturemiddle_x = 0;
int Picturemiddle_y = 0;

// own split function since arduino doesn't have a default one
void splitString(String input, char delimiter, String output[], int &count)
{
    int startIndex = 0;
    int endIndex = 0;
    count = 0;
    while (endIndex != -1)
    {
        endIndex = input.indexOf(delimiter, startIndex);
        output[count] = input.substring(startIndex, endIndex);
        startIndex = endIndex + 1;
        count++;
    }
}

// 2nd tokenizing function
void tokenize(char *str, char *delim, char **tokens)
{
    int i = 0;
    char *token = strtok(str, delim);
    while (token != NULL)
    {
        tokens[i] = token;
        i++;
        token = strtok(NULL, delim);
    }
}

void findNextPillar()
{
    String substrings[10]; // Create an array to store the substrings

    // Block
    P_color = 'Uknown';
    P_x = 0;
    P_y = 0;
    P_height = 0;
    P_width = 0;
    P_Bottom_edge = 0;
    P_Distance = 0;

    char str[] = "43 163 42 42 42";
    char *tokens[10];
    tokenize(str, " ", tokens);

    printf("first part: %s", tokens[1]);

    return 0;

    String data = raspi_get_data();
    int count = 0;
    splitString(data, ' ', substrings, count); // Split the data into substrings using a space delimiter
    P_color = substrings[0].charAt(0);
    P_x = substrings[1].toInt();
    P_y = substrings[2].toInt();
    P_height = substrings[3].toInt();
    P_width = substrings[4].toInt();
    P_Distance = substrings[5].toInt();
    return;
}
