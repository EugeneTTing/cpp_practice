#include<iostream>
#include<cctype>
#include<cstring>

using namespace std;

int encode_character(const char ch, char *braille);

void encode(const char *plaintext, char *brailler);

void print_braille(const char *plaintext, ostream& out);

int main()
{
    /****************** QUESTION 1 ******************/
    char braille[20];
    int size;

    size = encode_character('t', braille);
    cout << "'t' is encoded as: " << braille << " with size " << size << endl;

    size = encode_character('Z', braille);
    cout << "'Z' is encoded as: " << braille << " with size " << size << endl;

    size = encode_character('5', braille);
    cout << "'5' is encoded as: " << braille << " with size " << size << endl;

    size = encode_character('!', braille);
    cout << "'!' is encoded as: " << braille << " with size " << size << endl;
    cout << endl;

    /****************** QUESTION 2 ******************/
    char hello_braille[100];
    encode("Hello!", hello_braille);

    cout << "'Hello!' is encoded as:\n\"" << hello_braille << '"' << endl;
    cout << endl;

    /****************** QUESTION 2 ******************/
    print_braille("Hello!", cout);
    cout << endl;

    print_braille("Lj6.(", cout);
    cout << endl;

    return 0;
}

int encode_character(const char ch, char *braille)
{
    int length = 6;

    // overwrite braille string
    *braille = '\0';

    // check for capital
    if (isupper(ch))
    {
        length = 12;
        strcat(braille, ".....0");
    }

    // check for number
    if (isdigit(ch))
    {
        length = 12;
        strcat(braille, "..0000");
    }

    switch (tolower(ch)) 
    {
        case 'a':
        case '1':
            strcat(braille, "0.....");
            break;
        case 'b':
        case '2':
            strcat(braille, "00....");
            break;
        case 'c':
        case '3':
            strcat(braille, "0..0..");
            break;
        case 'd':
        case '4':
            strcat(braille, "0..00.");
            break;
        case 'e':
        case '5':
            strcat(braille, "0...0.");
            break;
        case 'f':
        case '6':
            strcat(braille, "00.0..");
            break;
        case 'g':
        case '7':
            strcat(braille, "00.00.");
            break;
        case 'h':
        case '8':
            strcat(braille, "00..0.");
            break;
        case 'i':
        case '9':
            strcat(braille, ".0.0..");
            break;
        case 'j':
        case '0':
            strcat(braille, ".0.00.");
            break;
        case 'k':
            strcat(braille, "0.0...");
            break;
        case 'l':
            strcat(braille, "000...");
            break;
        case 'm':
            strcat(braille, "0.00..");
            break;
        case 'n':
            strcat(braille, "0.000.");
            break;
        case 'o':
            strcat(braille, "0.0.0.");
            break;
        case 'p':
            strcat(braille, "0000..");
            break;
        case 'q':
            strcat(braille, "00000.");
            break;
        case 'r':
            strcat(braille, "000.0.");
            break;
        case 's':
            strcat(braille, ".000..");
            break;
        case 't':
            strcat(braille, ".0000.");
            break;
        case 'u':
            strcat(braille, "0.0..0");
            break;
        case 'v':
            strcat(braille, "000..0");
            break;
        case 'w':
            strcat(braille, ".0.000");
            break;
        case 'x':
            strcat(braille, "0.00.0");
            break;
        case 'y':
            strcat(braille, "0.0000");
            break;
        case 'z':
            strcat(braille, "0.0.00");
            break;
        case '.':
            strcat(braille, ".0..00");
            break;
        case ',':
            strcat(braille, ".0....");
            break;
        case ';':
            strcat(braille, ".00...");
            break;
        case '-':
            strcat(braille, "..0..0");
            break;
        case '!':
            strcat(braille, ".00.0.");
            break;
        case '?':
            strcat(braille, ".00..0");
            break;
        case '(':
        case ')':
            strcat(braille, ".00.00");
            break;
        default:
            strcat(braille, "......");
            break;
    }

    return length;
}

void encode(const char *plaintext, char *braille)
{
    if (*plaintext == '\0')
        return;

    char temp[20];
    int size;

    size = encode_character(*plaintext, temp);
    strcat(braille, temp);
    encode(plaintext + 1, braille + size);
}

void print_braille(const char *plaintext, ostream& out)
{
    char braille[256];
    braille[0] = '\0';
    encode(plaintext, braille);

    // count number of braille characters
    int count = strlen(braille) / 6;

    for (int i = 0 ; i < 3 ; ++i)
    {
        for (int j = 0 ; j < count; ++j)
        {
            out << braille[(6 * j) + i] << braille[(6 * j) + i + 3]
                << ' ';
        }
        out << '\n';
    }

    for ( ; *plaintext ; ++plaintext)
    {
        if (isupper(*plaintext) || isdigit(*plaintext))
            out << "   " << *plaintext << "  ";
        else
            out << *plaintext << "  ";
    }
    out << '\n';
}