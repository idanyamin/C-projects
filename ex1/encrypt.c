#include <stdio.h>

//size
#define LENGTH_OF_ARRAY 1024
const char GET_INPUT[] = "%1024c%n";

// error messages
const char INVALID_KEY[] = "INVALID ENCRYPTION KEY";
const char OUT_OF_RANGE[] = "KEY OUT OF RANGE";

//boundaries
const int MAX_KEY = 25;
const int MIN_KEY = -25;

// encrypt a single char
char getEncryptedChar(char input, int key, char startChar, char endChar);

// checks if encryption key is valid
int isEncryptionKeyValid(int isValid, int key);

/**
 * main
 * Encrypt user's input
 * @return 0
 */
int main()
{
    char buffer[LENGTH_OF_ARRAY];
    int key;
    int isValid = scanf("%d ", &key);
    // check if key is valid
    if (isEncryptionKeyValid(isValid, key) == 0)
    {
        return 1;
    }

    int length;
    while (scanf(GET_INPUT, &buffer[0], &length) != EOF)
    {
        for (int i = 0; i < length; i++)
        {
            if (buffer[i] >= 'a' && buffer[i] <= 'z')
            {
                buffer[i] = getEncryptedChar(buffer[i], key, 'a', 'z');
            }
            else if (buffer[i] >= 'A' && buffer[i] <= 'Z')
            {
                buffer[i] = getEncryptedChar(buffer[i], key, 'A', 'Z');
            }
            printf("%c", buffer[i]);
        }
    }
    return 0;
}

/**
 * checks if encryption key is valid
 * @param key encryption key
 * @param isValid scanf output
 * @return 1 if valid, 0 otherwise.
 */
int isEncryptionKeyValid(const int isValid, const int key)
{
    if (isValid == 0)
    {
        fprintf(stderr, INVALID_KEY);
        return 0;
    }
    if (!(key >= MIN_KEY && key <= MAX_KEY))
    {
        fprintf(stderr, OUT_OF_RANGE);
        return 0;
    }
    return 1;
}


/**
 * this function encrypt a single char
 * @param key the encryption shifting
 * @param input a char input
 * @param endChar the char at the end of the range
 * @param startChar the char at the start of the range
 * @return encrypted char
 */
char getEncryptedChar(const char input, const int key, const char startChar, const char endChar)
{
    int result = key + input;
    if (startChar <= input && input <= endChar)
    {
        if (result > endChar)
        {
            // fix rotation
            result = (result - endChar + startChar - 1);
        }
        else if (result < startChar)
        {
            // fix rotation
            result = (result + endChar - startChar + 1);
        }
    }
    return (char) result;
}