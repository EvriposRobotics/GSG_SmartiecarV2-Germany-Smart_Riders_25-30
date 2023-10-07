// #include "raspi.h"

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

void findNextPillar()
{
    String substrings[10]; // Create an array to store the substrings

    // Block
    P_color = 'U';
    P_x = 0;
    P_y = 0;
    P_height = 0;
    P_width = 0;
    P_Bottom_edge = 0;
    P_Distance = 0;

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
