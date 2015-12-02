#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>
#include <algorithm>

#define SYMBOL 256

bool read_from_file_instansly( std::ifstream *file, unsigned char *symbol ) {
	char temp;
	if ( ( *file ).get( temp ) ) {
		symbol[0] = temp;
		
		return true;
	} else {
		return false;
	}
}

double mean_squared_error( std::ifstream *file, std::ifstream *file2, int size ) {
	unsigned char symbol[1];
	unsigned char symbol2[1];
	
	symbol[0] = 0x00;
	symbol2[0] = 0x00;
	
	long long int current = 0;
	
	for ( int iter=0; iter<size; ++iter ) {
		read_from_file_instansly( &*file, symbol );
		read_from_file_instansly( &*file2, symbol2 );
		
		current += ( ( int )symbol[0] - ( int )symbol2[0] ) * ( ( int )symbol[0] - ( int )symbol2[0] );
		
//		if ( current < 0 )
//			std::cout << current << " " << iter << '\n';
	}
	
//	std::cout << current << '\n';
	
	return 1.0 / size * current;
}

double peak_signal_to_noise_ratio( double mse ) {
	return 10.0 * log10( ( SYMBOL - 1 ) * ( SYMBOL - 1 ) / mse );
}

int main( int argc, char* argv[] ) {
	/** argv = {
	 *      0 := this program (.exe)
	 *      1 := filname 1 (.pgm) original
	 *      2 := filname 2 (.pgm / .restore / .restorelzw / .restorelzwo) restore
	 *  }
	 */
	
	if ( argc != 3 ) {
		printf( "\aUsage: %s <filename original> <filename restore>\n", argv[0] );
		
		return 1;
		
	} else {
		std::ifstream in;
		in.open( argv[1], std::ios::in | std::ios::binary );
		
		std::ifstream in2;
		in2.open( argv[2], std::ios::in | std::ios::binary );
		
		char temp[128];
		
		int width, height;
		int i = 0;
		do {
			in.getline( temp, 128 );
			if ( i == 1 ) {
				if ( strstr(temp, " ") != NULL ) {
					sscanf( temp, "%d %d", &width, &height );
				} else {
					sscanf( temp, "%d", &width );
					in.getline( temp, 128 );
					sscanf( temp, "%d", &height );
				}
			}
			
			if ( temp[0] == '#' )
				continue;
			else
				i++;
			
		} while( i < 3 );
		
		i=0;
		do {
			in2.getline( temp, 128 );
			if ( temp[0] == '#' )
				continue;
			else
				i++;
			
		} while( i < 3 );
		
		int size = width * height;
		
		double mse = mean_squared_error( &in, &in2, size );
		
		std::cout << "MSE := " << mse << '\n';
		
		double psnr = peak_signal_to_noise_ratio( mse );
		
		std::cout << "PSNR := " << psnr << " dB\n\a";
	}
	
	return 0;
}
