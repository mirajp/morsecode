//text2morse -- Read a line of text and play the Morse equivalent.
//Miraj Patel
//ECE161 Prof. Kirtman

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "morsetable.h"
#include "dsp.h"

#include <fcntl.h>
#include <sys/soundcard.h>

#define SAMP_RATE 48000

#define PI (3.14159265)



int main(int argc, char *argv[])
{
	int i;
	unsigned int dit = 500;
	unsigned int freq = 200;
	char linestring[16];
	int ditlength; // This will be the primary length = 1 unit.   For the dots.  --- CONVERTED DIRECTLY FROM USER INPUT
	int dahlength; // This will be the other length = 3 units.    For the dashes.
	double elementgap; // The time between two consecutive dots/dashes within the same character     = 1 dot/1 unit.
	double spacetime; // The gap between characters/letters entered = 3 units.
	double wordspace; // The gap between words (BASICALLY A SPACE) = 7 units.
	
	char response[5];
	response[0] = 'Y';
	
	if( argc == 1 ) // 1 for executeable name, 2nd is the input filename, 3rd is the integer
	{
        fprintf(stderr, "Using default dit time = %dms and frequency = %dHz\n", dit, freq);
	}
	
    else if( argc == 2 )
	{
		dit = atoi( argv[1] );
        fprintf(stderr, "Using default frequency = %dHz\n", freq);
	}
    
    else 
	{		
        dit = atoi( argv[1] );
        freq = atoi( argv[2] );
        if (argc > 3)
            fprintf(stderr, "Unknown parameters passed. Usage expected: %s <dit_time_in_ms>  <frequency>\n", argv[0]);
	}
	if( dit >= 1000 )
	{
		printf("Dit time entered is rather high (greater than 1 second) - continuing nonetheless...\n");
	}
	ditlength = ((dit*48000)/1000); // dit = dot = 0 in binary representation
	dahlength = 3*ditlength; // dah = dash = 1 in binrary representation
	elementgap = (dit/1000); // this gap is in seconds... will convert later for microsecond usleep function
	spacetime = elementgap*3; // similar to above
	wordspace = elementgap*7; // similar to above
//	printf("dit = %d\n", dit);
//	printf("ditlength = %d\n", ditlength);
//	printf("dahlength = %d\n", dahlength);
	
	
while( (response[0] == 'Y') || (response[0] == 'y') || (response[0] == '1') )  // for rerunning
{
	printf("\nEnter text: (Indicate prosign strings with the use of a '^' or a '|')\n");
	fgets(linestring, sizeof(linestring), stdin); // Read in the line
//	fgets(linestring, sizeof(linestring), stdin); // Read in the line
//	linestring[(strlen(linestring)-1)] = '\0'; // Change new line character into NULL
	
	for(i=0; (i<strlen(linestring)); i++)
	{
		if( linestring[i] == ' ')
			usleep(wordspace*1000000); // Rest for 7 units after every word... Word end denoted by a space. times 1,000,000 since in micro
		
		else
		{
			int j;
			int cAscii = (int)linestring[i]; // The index that will point to location of character in the morsetab array in the header file
			int dec = Morse_table[cAscii]; // Beginning to take that hex value from morsetable and convert dec, then binary 
			unsigned char bin[9];
			//printf("dec = %d\n", dec); // Just to make sure right decimal number from the hex
			
			if ( (linestring[i+1] == '^') || (linestring[i+1] == '|') )
			{
				spacetime = 0; // Eliminate the pause between characters if it is indicated to be a prosign
				i++; // don't want to read in the ^ or | as an actual character of the inputted line -- the end of this FOR loop will increment over it
			}
			else if ( ( (linestring[i] == 'S') || (linestring[i] == 's') ) && ( (linestring[i+1] == 'O') || (linestring[i+1] == 'o') ) && ( (linestring[i+2] == 'S') || (linestring[i+2] == 's') ) ) // If three consecutive characters are SOS
			{
				bin[0] = '0';
				bin[1] = '0';
				bin[2] = '0';
				bin[3] = '1';
				bin[4] = '1';
				bin[5] = '1';
				bin[6] = '0';
				bin[7] = '0';
				bin[8] = '0';
				
				for ( j=0; j<9; j++ )
				{
					int k;
					
					if ( bin[j] == '0')	// For a "." (dot or dit)
					{
						unsigned short s[ditlength];
						
						for(k=0; k < ditlength; k++)
							s[i] = 32767*(sin(2.0*PI*freq*i/SAMP_RATE));
						
						dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
						dsp_write(s, sizeof(short), ditlength);
						dsp_sync();	
						dsp_close();
                         // the gap between each element (multiplied by 1,000,000 is because usleep in in microseconds)
						usleep(elementgap*1000000);
					}
					else if ( bin[j] == '1') // For a "-" (dash or dah)
					{
						unsigned short s[dahlength];
						
						for(k=0; k < dahlength; k++)
							s[i] = 32767*(sin(2.0*PI*freq*i/SAMP_RATE));
						
						dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
						dsp_write(s, sizeof(short), dahlength);
						dsp_sync();	
						dsp_close();
						usleep(elementgap*1000000);
					}
					
				}
				i = i+2;
				break;
			}
			else
				;
			
			// making the binary string back from the hex that will represent the length+dits and dahs
			for( j=0; j<8; j++)
			{
				if( dec - pow(2,(7-j)) >=0)
				{
					bin[j] = '1';
					dec = (dec - pow(2,(7-j)));
				}
				else
					bin[j] = '0';
			}
			
			if ( bin[0] == '1' && bin[1] == '1' ) // Things of length 6
			{
				for( j=2; j<8; j++) // Look at the next 6 dits/dahs
				{
					int k;
					
					if ( bin[j] == '0')	// For a "." (dot or dit)
					{
						unsigned short s[ditlength];
						
						for(k=0; k < ditlength; k++)
							s[i] = 32767*(sin(2.0*PI*freq*i/SAMP_RATE));
						
						dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
						dsp_write(s, sizeof(short), ditlength);
						dsp_sync();	
						dsp_close();
						usleep(elementgap*1000000);
					}
					else if ( bin[j] == '1') // For a "-" (dash or dah)
					{
						unsigned short s[dahlength];
						
						for(k=0; k < dahlength; k++)
							s[i] = 32767*(sin(2.0*PI*freq*i/SAMP_RATE));
						
						dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
						dsp_write(s, sizeof(short), dahlength);
						dsp_sync();	
						dsp_close();
						usleep(elementgap*1000000);
					}
					
				}
				usleep(spacetime*1000000);
			}
			
			else if ( bin[0] == '1' && bin[1] == '0' && bin[2] == '1' ) // Things of length 5
			{
				for( j=3; j<8; j++) // Look at the next 5 dits/dahs
				{
					int k;
					
					if ( bin[j] == '0')	// For a "." (dot or dit)
					{
						unsigned short s[ditlength];
						
						for(k=0; k < ditlength; k++)
							s[i] = 32767*(sin(2.0*PI*freq*i/SAMP_RATE));
							
						dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
						dsp_write(s, sizeof(short), ditlength);
						dsp_sync();	
						dsp_close();
						usleep(elementgap*1000000);
					}
					else if ( bin[j] == '1') // For a "-" (dash or dah)
					{
						unsigned short s[dahlength];
						
						for(k=0; k < dahlength; k++)
							s[i] = 32767*(sin(2.0*PI*freq*i/SAMP_RATE));
						
						dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
						dsp_write(s, sizeof(short), dahlength);
						dsp_sync();	
						dsp_close();
						usleep(elementgap*1000000);
					}
				}
				usleep(spacetime*1000000);
			}
			
			else if ( bin[0] == '1' && bin[1] == '0' && bin[2] == '0' ) // Things of length 4
			{
				for( j=3; j<7; j++) // Look at the next 4 dits/dahs
				{
					int k;
					
					if ( bin[j] == '0')	// For a "." (dot or dit)
					{
						unsigned short s[ditlength];
						
						for(k=0; k < ditlength; k++)
							s[i] = 32767*(sin(2.0*PI*freq*i/SAMP_RATE));
						
						dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
						dsp_write(s, sizeof(short), ditlength);
						dsp_sync();	
						dsp_close();
						usleep(elementgap*1000000);
					}
					else if ( bin[j] == '1') // For a "-" (dash or dah)
					{
						unsigned short s[dahlength];
						
						for(k=0; k < dahlength; k++)
							s[i] = 32767*(sin(2.0*PI*freq*i/SAMP_RATE));
						
						dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
						dsp_write(s, sizeof(short), dahlength);
						dsp_sync();	
						dsp_close();
						usleep(elementgap*1000000);
					}
				}
				usleep(spacetime*1000000);
			}
			
			else if ( bin[0] == '0' && bin[1] == '1' && bin[2] == '1' ) // Things of length 3
			{
				for( j=3; j<6; j++) // Look at the next 3 dits/dahs
				{
					int k;
					
					if ( bin[j] == '0')	// For a "." (dot or dit)
					{
						unsigned short s[ditlength];
						
						for(k=0; k < ditlength; k++)
							s[i] = 32767*(sin(2.0*PI*freq*i/SAMP_RATE));
						
						dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
						dsp_write(s, sizeof(short), ditlength);
						dsp_sync();	
						dsp_close();
						usleep(elementgap*1000000);
					}
					else if ( bin[j] == '1') // For a "-" (dash or dah)
					{
						unsigned short s[dahlength];
						
						for(k=0; k < dahlength; k++)
							s[i] = 32767*(sin(2.0*PI*freq*i/SAMP_RATE));
						
						dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
						dsp_write(s, sizeof(short), dahlength);
						dsp_sync();	
						dsp_close();
						usleep(elementgap*1000000);
					}
				}
				usleep(spacetime*1000000);
			}
			
			else if ( bin[0] == '0' && bin[1] == '1' && bin[2] == '0' ) // Things of length 2
			{
				for( j=3; j<5; j++) // Look at the next 2 dits/dahs
				{
					int k;
					
					if ( bin[j] == '0')	// For a "." (dot or dit)
					{
						unsigned short s[ditlength];
						
						for(k=0; k < ditlength; k++)
							s[k] = 32767*(sin(2.0*PI*freq*k/SAMP_RATE));
						
						dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
						dsp_write(s, sizeof(short), ditlength);
						dsp_sync();	
						dsp_close();
						usleep(elementgap*1000000);
					}
					else if ( bin[j] == '1') // For a "-" (dash or dah)
					{
						unsigned short s[dahlength];
						
						for(k=0; k < dahlength; k++)
							s[k] = 32767*(sin(2.0*PI*freq*k/SAMP_RATE));
						
						dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
						dsp_write(s, sizeof(short), dahlength);
						dsp_sync();	
						dsp_close();
						usleep(elementgap*1000000);
					}
				}
				usleep(spacetime*1000000);
			}
			
			else if ( bin[0] == '0' && bin[1] == '0' && bin[2] == '1' ) // Things of length 1
			{
				for( j=3; j<4; j++) // Look at the next dits/dahs
				{
					int k;
					
					if ( bin[j] == '0')	// For a "." (dot or dit)
					{
						unsigned short s[ditlength];
						
						for(k=0; k < ditlength; k++)
							s[i] = 32767*(sin(2.0*PI*freq*i/SAMP_RATE));
						
						dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
						dsp_write(s, sizeof(short), ditlength);
						dsp_sync();	
						dsp_close();
						usleep(elementgap*1000000);
					}
					else if ( bin[j] == '1') // For a "-" (dash or dah)
					{
						unsigned short s[dahlength];
						
						for(k=0; k < dahlength; k++)
							s[i] = 32767*(sin(2.0*PI*freq*i/SAMP_RATE));
						
						dsp_open("/dev/dsp", O_WRONLY, AFMT_S16_NE, 1, SAMP_RATE);
						dsp_write(s, sizeof(short), dahlength);
						dsp_sync();	
						dsp_close();
						usleep(elementgap*1000000);
					}
				}
				usleep(spacetime*1000000);
			}
		}
	}
	printf("\nWould you like you enter encode another line?(Y/y = Yes)  ");
	fgets(response, 3, stdin);
}
return 0;
}