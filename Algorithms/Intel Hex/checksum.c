#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void computeCheckSum(char *record)
{
	uint8_t check_sum = 0;
	record++;

	while (record[1] != '\n')
	{
		check_sum += *record;
	    record++;
	}
	*record = ~check_sum + 1;
}

void printRecord(char *record)
{
	while (*record != '\n')
	{
		printf("%1x", *record);
	    record++;
	}
	printf("\n");
}

int main()
{
	char record[] = ":\x10\x45\x50\x00\x06\x81\x83\x20\x7F\x0F\xF0\x7F\xC0\x21\x05\x01\x30\x1C\x40\x36\x9B\n";
	computeCheckSum(record);
	printRecord(record);

	return EXIT_SUCCESS;
}
