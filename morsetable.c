// Character input to ASCII
// Miraj Patel
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//main(int argc, char *argv[])

int main(int argc, char *argv[])
{
	FILE *inputfile;
	
	if( argc < 2 )
	{
		printf("usage expected: %s <filename>\n", argv[0]);
		exit(1);
	}
	if( (inputfile = fopen(argv[1], "r+")) == NULL)
	{
		printf("Cannot open the file!\n");
		exit(1);
	}
	char c;
	int i;
	int len;
	char s[7];
	long int lengthbit;
	int upper;

//	unsigned char *Morse_tab[256];
	
	unsigned char Morse_table[256]; 
	for(i = 0; i < 256; i++)
		Morse_table[i] = 0;
		
	while( (c = fgetc( inputfile)) != EOF )
	{
		int cAscii = (int)c;
//		printf("Character read was: '%c' and its ASCII value is: %d", c, cAscii);
		char ch = c;
		
		c = fgetc( inputfile );
			
		if ( c != ' ')
		{
			printf("Space character after character not found!\n");
			break;
		}
//		int len = 0;
//Continuing because:		printf("There was a space found\n");
//		i =0;

		for (i=0, len=0; i < 7; len++, i++)	
			{
				c = fgetc( inputfile);
				if ( c == '\r' )
					break;
				else if ( c == '\n' )
					break;
				else
					s[i] = c;
			}

//		for ( i=0; i < len; i++)    /*Making sure the correct stuff got put into the array*/
//			printf("s[%d] = %c\n", i, s[i]);
		
//		printf("Length = %d", len);  /* Checking length*/
	
		if ( len == 1)
			lengthbit = 00100000.;
		else if ( len == 2)
			lengthbit = 01000000.;
		else if ( len == 3)
			lengthbit = 01100000.;
		else if ( len == 4)
			lengthbit = 10000000;
		else if ( len == 5)
			lengthbit = 10100000;
		else if ( len == 6)
			lengthbit = 11000000;
				
		if( len < 6)
			upper = 5;
		else if( len == 6)
			upper = 6;
		
		unsigned int binaryintermediary = 0;
		
		int lenfor = len;
		for ( i=0; i < lenfor; i++)
		{	
//			while( len != 0)
//			{
				if ( s[i] == '-')
					binaryintermediary = binaryintermediary + (1*(pow(10,(upper-i-1))));
				else if ( s[i] == '.')
					binaryintermediary = binaryintermediary + 0;
				len--;
//			}
			if ( len == 0)
				break;
		}
		binaryintermediary = lengthbit + binaryintermediary;
//		printf("\nbinaryintermediary = %08d\n", binaryintermediary);   /* Here lies the binary representation of the morse character */
		
		int dec = 0;
		if (binaryintermediary - pow(10,7) >= 0)
			{
				dec = pow(2,7);
				binaryintermediary = binaryintermediary - pow(10,7);
			}
		if (binaryintermediary - pow(10,6) >= 0)
			{
				dec = dec + pow(2,6);
				binaryintermediary = binaryintermediary - pow(10,6);
			}
		if (binaryintermediary - pow(10,5) >= 0)
			{
				dec = dec + pow(2,5);
				binaryintermediary = binaryintermediary - pow(10,5);
			}
		if (binaryintermediary - pow(10,4) >= 0)
			{
				dec = dec + pow(2,4);
				binaryintermediary = binaryintermediary - pow(10,4);
			}
		if (binaryintermediary - pow(10,3) >= 0)
			{
				dec = dec + pow(2,3);
				binaryintermediary = binaryintermediary - pow(10,3);
			}
		if (binaryintermediary - pow(10,2) >= 0)
			{
				dec = dec + pow(2,2);
				binaryintermediary = binaryintermediary - pow(10,2);
			}
		if (binaryintermediary - pow(10,1) >= 0)
			{
				dec = dec + pow(2,1);
				binaryintermediary = binaryintermediary - pow(10,1);
			}
		if (binaryintermediary - pow(10,0) >= 0)
			dec = dec + pow(2,0);
		
//		printf("Decimal value: %d\n", dec);
		
		if( ch >= 'A' && c <= 'Z') // Defining for the lowercases
		{
			int lowerc = cAscii + 32;
			Morse_table[lowerc] = dec;
//			char chl = ch + 32;
//			printf("Morse_table[%d (%c)] = 0x%X\n", lowerc, chl, dec);
		}
		
		if( ch >= 'a' && c <= 'z') // Defining for the uppercases
		{
			int upperc = cAscii - 32;
			Morse_table[upperc] = dec;
//			char chl = ch - 32;
//			printf("Morse_table[%d (%c)] = 0x%X\n", lowerc, chl, dec);
		}
		
		Morse_table[cAscii] = dec;
//		printf("Morse_table[%d (%c)] = 0x%X\n", cAscii, ch, dec);   /* this and above printf line for other part of assignment? */

			
		fgetc( inputfile); // to jump over the new line, and thus not storing in the array for the next round
	}
	
	printf("unsigned char Morse_table[256] = \n {");
	for (i=0; i<256; i++)
	{
		printf("0x%X", Morse_table[i]);
		if ( i<255)
			printf(",");
		else if (i == 255)
			printf("};");
	}
	
	return 0;
}		
	