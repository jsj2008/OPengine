#include "include/OPstream.h"
#include <stdio.h>

int main(){
	OPstream* stream = (OPstream*)OPalloc(sizeof(OPstream));
	stream->Data = (ui8*)OPalloc(sizeof(OPint) * 10);
	stream->Length = 10 * sizeof(OPint);
	stream->_pointer = 0;

	// write some shit in
	{
		OPint data[3] = { 1, 33, 7 };
		int i = 0;

		for(i = 0; i < 30; i ++)
			OPwrite(stream, &data, sizeof(OPint) * 3);
	}

	OPseek(stream, 0);

	// read some shit!
	{
		int i, *j, k;

		for(i = 0; i < 30; i++){
			j = (int*)OPread(stream, sizeof(int) * 3);

			for(k = 0; k < 3; k++){
				printf("%d ", j[k]);
			} printf("\n");
		}
	}
	return 0;
}
