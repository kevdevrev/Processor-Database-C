//Kevin Galeano
//4-6-2018
//5 hour challenge
#include "kevheader.h"

//structs

typedef struct {
	int coreCount;
	int threadCount;
	float clock;
	float boostClock;
	int wattageTDP;

}SPECS;

typedef struct {
	char processorBrand[8];
	char processorName[12];
	char SKU[5];
	float processorPrice;
	int count;
	SPECS specs;

}PROCESSOR;


//prototypes
void addProcessor(PROCESSOR **cpu, int *eSize);
void displayCPUTypes(PROCESSOR **cpu, int *eSize);
void displayMenu();
void displaySearchResult(PROCESSOR **cpu, int i);
void displaySortingMenu();
void formatCategoryDisplay(int sizeOfCatString, int maxPosLength);
void formatCPUdisplay(int lengthOfS, int tempSize);
char getChoice();
float floatValidator(char msg[], float compare, int max);
int getSize();
int intValidator(char msg[], int max);
PROCESSOR *loadCPUs(int *size, int *eSize, char *opened);
void manageQuantity(PROCESSOR **cpu, int eSize, char msg[]);
void myFlush();
void removeProcessor(PROCESSOR **cpu, int *eSize);
void saveCPUs(PROCESSOR **cpu, int size, int eSize);
void searchCPUs(PROCESSOR **cpu, int eSize);
void sortProcessors(PROCESSOR **cpu, int eSize, char flag);
void stringValidator(int *eSize, int lengthDesired, PROCESSOR *temp, char msg[]);


main() {
	PROCESSOR **cpu;
	int size = 0;
	int eSize = 0;
	char opened = 'N';
	cpu = loadCPUs(&size, &eSize, &opened);
	if (opened == 'N') {
		size = getSize();
		cpu = calloc(size, sizeof(PROCESSOR*));
		printf("Starting storage: %i\n", size);
	}
	char choice;
	do {
		if (size < eSize) {
			size = eSize;
		}//dont call tech support im sleeping and i was feeling generous
		displayMenu();
		choice = getChoice();
		printf("eSize: %i\n", eSize);
	
		switch (choice) {
			case 'A':
				addProcessor(cpu, &eSize, size);
				break;
			case 'B':
				removeProcessor(cpu, &eSize);
				break;
			case 'C':
				manageQuantity(cpu, eSize, "increase");
				break;
			case 'D':
				manageQuantity(cpu, eSize, "decrease");
				break;
			case 'E':
				CLS;
				displayCPUTypes(cpu, &eSize);
				PAUSE;
				break;
			case 'F':
				searchCPUs(cpu, eSize);
				break;
			case 'G':
				displaySortingMenu();
				do {
					displaySortingMenu();
					choice = getChoice();
					switch (choice) {
						case 'A':
							sortProcessors(cpu, eSize, 'B');
							PAUSE;
							break;
						case 'B':
							sortProcessors(cpu, eSize, 'P');
							PAUSE;
							break;
						case 'C':
							sortProcessors(cpu, eSize, 'O');
							PAUSE;
							break;
						case 'E':
							break;
						default:
							printf("Please enter a valid input.\n");
							PAUSE;
							CLS;
							break;
					}
				} while (choice != 'E');

				break;
			case 'Q':
				printf("Saving data...\n");
				saveCPUs(cpu, size, eSize);
				printf("Shutting down...\n");
				printf("Goodbye!\n");
				break;
			default:
				printf("Please enter a valid input.\n");
				break;
		}
	} while (choice != 'Q');
	//end menu

}//end main
////functions

PROCESSOR *createNode() {
	int *node = calloc(1, sizeof(PROCESSOR));
	if (node == NULL) {
		printf("No room for node.\n");
		PAUSE;
		exit(-1);
	} // end if
	return node;
}//end createNode

void addProcessor(PROCESSOR **cpu, int *eSize) {
	cpu[*eSize] = createNode();

	stringValidator(eSize, sizeof(cpu[*eSize]->processorBrand), cpu[*eSize]->processorBrand, "Brand");
	stringValidator(eSize, sizeof(cpu[*eSize]->processorName), cpu[*eSize]->processorName, "Name");
	stringValidator(eSize, sizeof(cpu[*eSize]->SKU), cpu[*eSize]->SKU, "SKU");
	cpu[*eSize]->specs.coreCount = intValidator("number of Cores", 256);
	cpu[*eSize]->specs.threadCount = intValidator( "number of Threads", (cpu[*eSize]->specs.coreCount * 2));
	cpu[*eSize]->specs.clock = floatValidator("Base Clock", 0.1, 5.5);
	cpu[*eSize]->specs.boostClock = floatValidator("Boost Clock", cpu[*eSize]->specs.clock, (cpu[*eSize]->specs.clock+(1.2)));
	cpu[*eSize]->specs.wattageTDP = intValidator("Wattage / TDP", 999);
	cpu[*eSize]->processorPrice = floatValidator("price", 0.01, 10000);
	cpu[*eSize]->count = 0;

	*eSize = *eSize + 1;
}//end addProcessor

void manageQuantity(PROCESSOR **cpu, int eSize, char msg[]) {
	int temp = 0;
	char passOrFail = 'Y';
	int check = 0;
	char reinput = 'N';
	int choice = ' ';
	do {
		passOrFail = 'Y';
		reinput == 'N';
		temp = 0;
		printf("Which processor would you like to %s quantity of?\n", msg);
		displayCPUTypes(cpu, &eSize);
		choice = intValidator("desired processer.", eSize) - 1;
		temp = intValidator(msg, INT_MAX);
		if (strcmp(msg, "increase") == 0) {
			cpu[choice]->count += temp;
			passOrFail = 'Y';
		}
		else {
			check = cpu[choice]->count;
			if (check - temp < 0) {
				do {
					printf("You can't have negative storage. Press Y to reinput and N to exit.");
					reinput = getChoice();
					if (reinput != 'N' && reinput != 'Y') {
						printf("Please press Y to reinput and N to exit.");
					}
				} while (reinput != 'N' && reinput != 'Y');
				passOrFail = 'N';
			}
			else {
				cpu[choice]->count -= temp;
				passOrFail = 'Y';
			}

		}//end else
	} while (passOrFail  != 'Y' && reinput == 'Y' );
}//end addQuantity

void displayCPUTypes(PROCESSOR **cpu, int *eSize) {
	//define lengths
	char coreThread[10];
	sprintf(coreThread, "%d / %d", 0, 0);
	char clockString[12];
	sprintf(clockString, "%.1f/%.1f ghz", 0, 0);
	char TDPString[7];
	sprintf(TDPString, "%i", 0);
	char quantityString[12];
	sprintf(quantityString, "%i", 0);


	//TOP DISPLAY ensure space added after each printf
	printf("\n# | Brand");
	formatCategoryDisplay(sizeof("Brand"), 8);
	//name
	printf(" | ");
	printf("Name");
	formatCategoryDisplay(sizeof("Name"), 12);
	//SKU
	printf(" | ");
	printf("SKU");
	formatCategoryDisplay(sizeof("SKU"), 6);
	//core/thread
	printf(" | ");
	printf("Cores/Threads");
	formatCategoryDisplay(sizeof("Core/Threads"), 12);
	//clockString
	printf(" | ");
	printf("Base/Boost Clocks");
	formatCategoryDisplay(sizeof("Base/Boost Clocks"), 10);
	//TDP
	printf(" | ");
	printf("TDP/W");
	formatCategoryDisplay(strlen("TDP/W"), strlen("TDP/W"));
	//Count
	printf(" | ");
	printf("Quantity");
	formatCategoryDisplay(strlen("Quantity"), 8);
	//price
	printf(" | ");
	printf("Price");
	formatCategoryDisplay(strlen("Price"), 8);
	printf("\n");

	//thing
	for (int i = 0; i < 100; i++) {
		printf("=");
	}

	for (int i = 0; i < *eSize; i++) {
		//string formatters
		memset(coreThread, 0, strlen(coreThread));
		memset(clockString, 0, strlen(clockString));
		memset(TDPString, 0, strlen(TDPString));
		memset(quantityString, 0, strlen(quantityString));

		sprintf(coreThread, "%d / %d", cpu[i]->specs.coreCount, cpu[i]->specs.threadCount);
		sprintf(clockString, "%.1f/%.1f ghz", cpu[i]->specs.clock, cpu[i]->specs.boostClock);
		sprintf(TDPString, "%i", cpu[i]->specs.wattageTDP);
		sprintf(quantityString, "%i", cpu[i]->count);

		//BOTTOM DISPLAY
		//number
		printf("\n%i", i + 1);
		//brand
		printf(" | ");
		printf("%s", cpu[i]->processorBrand);
		formatCPUdisplay(strlen(cpu[i]->processorBrand), sizeof(cpu[i]->processorBrand));
		//name'
		printf(" | ");
		printf("%s", cpu[i]->processorName);
		formatCPUdisplay(strlen(cpu[i]->processorName), sizeof(cpu[i]->processorName));
		//SKY
		printf(" | ");
		printf("%s", cpu[i]->SKU);
		formatCategoryDisplay(strlen(cpu[i]->SKU), sizeof(cpu[i]->SKU));
		//core/thread
		printf(" | ");
		printf("%s", coreThread);
		formatCPUdisplay(strlen(coreThread), strlen("Cores/Threads"));
		//clockString
		printf(" | ");
		printf("%s", clockString);
		formatCPUdisplay(strlen(clockString), strlen("Base/Boost Clocks"));
		//TDP
		printf(" | ");
		printf("%i", cpu[i]->specs.wattageTDP);
		formatCPUdisplay(strlen(TDPString), strlen("TDP/W"));
		//Quantitiy
		printf(" | ");
		printf("%i", cpu[i]->count);
		formatCPUdisplay(strlen(quantityString), strlen("Quantity"));
		//Quantitiy
		printf(" | ");
		printf("$%.2f", cpu[i]->processorPrice);
		formatCPUdisplay(strlen(quantityString), strlen("Price"));
	}
	printf("\n");
}//end displayVoter

void displayMenu() {
	CLS;
	printf("Kevin's Processor Emporium\n");
	printf("[A] Initilize CPU\n");
	printf("[B] Delete CPU\n");
	printf("[C] Add Quantity\n");
	printf("[D] Remove Quantity\n");
	printf("[E] Display CPUs by order inputed\n");
	printf("[F] Search for SKU...\n");
	printf("[G] Show sort options...\n");
	printf("[Q] End Program\n");
}//end displayMenu

void displaySortingMenu() {
	CLS;
	printf("[A] Display CPUs by Quantity\n");
	printf("[B] Display CPUs by Price\n");
	printf("[C] Display CPUs by Brand and Name and SKU\n");
	printf("[E] Exit\n");
}//enddisplay sorting menu
void formatCategoryDisplay(int sizeOfCatString, int maxPosLength) {
	if (maxPosLength > sizeOfCatString) {
		for (int i = 0; i < (maxPosLength - sizeOfCatString) + 1; i++) {
			//	printf("%ic", i);
					//printf("test");
			printf(" ");

		}//end for
	}
	else if (sizeOfCatString < maxPosLength) {
		for (int i = 0; i < (sizeOfCatString - maxPosLength) + 1; i++) {
			//printf("%i", i);
		}
	}
}//end formatCate
void formatCPUdisplay(int lengthOfArray, int tempSize) {
	//printf("\nCPU BRAND: %s", temp);
//	printf("\n\n %i and %i\n\n", tempSize, s);
	//printf("lengthOfArray: %i vs tempSize: %i\n", lengthOfArray, tempSize);
	//PAUSE;
	if (lengthOfArray > tempSize) {
		for (int i = 0; i < (lengthOfArray - tempSize); i++) {
			//	printf("%i1", i);
				//printf("test");
			printf(" ");

		}//end for
	}
	else if (tempSize > lengthOfArray) {
		for (int i = 0; i < tempSize - lengthOfArray; i++) {
			//	printf("%i2", i);
				//printf("test");
			printf(" ");
		}//end for

	}
	//	printf("test");
}//end formatCPUdisplay

float floatValidator(char msg[], float compare, int max) {
	float temp = 0.0;
	//printf("compare: %f", compare);
	//PAUSE;
	do {
		printf("Please enter the %s.\n", msg);
		scanf("%f", &temp);
		FLUSH;
		if (temp < compare && compare == 0) {
			printf("Please enter a value greater than zero.\n");
		}
		else if (temp < compare) {
			printf("The %s must be larger than %.1f\n", msg, compare);
		}
		else if (temp > max & strcmp(msg, "Boost Clock") == 0) {
			printf("Unrealistic boost clock. \n");
		}
	} while (temp < compare || temp > max || temp < 0);

	return temp;

}//end float validator
char getChoice() {
	char choice;
	scanf("%c", &choice); FLUSH;
	return toupper(choice);
} // end getChoice
int getSize() {
	int size;
	char valid = 'Y';
	do {
		printf("Please enter the number of the initial batch of CPUs.");
		if (scanf("%i", &size) == 0) { // invalid value when the scanf returns a zero value
			valid = 'N';
			printf("\n\nWe need processors. Go order at least one.\n\n");
		}
		else
			valid = 'Y';
		FLUSH;
	} while (valid == 'N' || size <= 0);
	return size;
} // end getSize
int intValidator(char msg[], int max) {
	int test;
	do {
		printf("Please enter the %s\n", msg);
		while (scanf("%i", &test) != 1) {
			printf("Please enter an integer: \n");
			FLUSH;
		}
		//printf("temp: %i",test);
		if (test < 0 || test > max) {
			printf("Please enter a valid %s\n below %i.", msg, max);
		}
	} while (test < 0 || test >max);
	return test;
}//end intValidator

PROCESSOR *loadCPUs(int *size, int *eSize, char *opened) {
	PROCESSOR **loadCPU = NULL;
	FILE *cpuFile = fopen("cpus.bin", "rb");
	if (cpuFile == NULL) {
		*opened = 'N';
		printf("Not able to open a saved data file....\n");
		PAUSE;
		return loadCPU;
	} // 
	// Get the size
	fread(size, sizeof(int), 1, cpuFile);
	// Get the effective size
	fread(eSize, sizeof(int), 1, cpuFile);

	// Create the table of contents array or the ** array
	loadCPU = calloc(*size, sizeof(PROCESSOR*));
	if (loadCPU == NULL) {
		printf("Error in memory.\n");
		PAUSE;
		exit(-1);
	} // end if

// Create and populate the NODES
	for (int i = 0; i < *eSize; i++) {
		loadCPU[i] = createNode();
		fread(loadCPU[i], sizeof(PROCESSOR), 1, cpuFile);
	} // end for 
	printf("Processors Successfully Loaded\n");
	*opened = 'Y';
	PAUSE;
	return loadCPU;
} // end loadStuff
void removeProcessor(PROCESSOR **cpu, int *eSize) {
	int bottom = *eSize - 1;
	int cpuChoice;
	displayCPUTypes(cpu, eSize);
	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	cpuChoice = intValidator("desired CPU to delete", &eSize);
	printf("cpuchoice entered: %i", cpuChoice);
	cpuChoice--;
	printf("cpuchoice fixed: %i", cpuChoice);

	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	if (cpuChoice > 0) {
		for (int i = cpuChoice; i < bottom; i++) {
			//	cpu[i] = cpu[i + 1];
			memmove(cpu[i], cpu[i + 1], sizeof(PROCESSOR*));
		}
	}
	printf("loop pass.");
	free(cpu[bottom]); //clears the last value in cpu array, where the desired deletion was
	//subtracts 1 from eSize
	*eSize = *eSize - 1;
}//end remove processor
void saveCPUs(PROCESSOR **cpu, int size, int eSize) {
	FILE *saveCPUs = fopen("cpus.bin", "wb");
	if (saveCPUs == NULL) {
		printf("Sorry I cannot save anything...error in file\n");
		PAUSE;
		return;
	} // end if

	// Save the Size
	fwrite(&size, sizeof(int), 1, saveCPUs);

	// Save the eSize
	fwrite(&eSize, sizeof(int), 1, saveCPUs);

	// Save the NODES
	for (int i = 0; i < eSize; i++) {
		fwrite(cpu[i], sizeof(PROCESSOR), 1, saveCPUs);
		free(cpu[i]);
	} // end for

// Close out the memory for the table of contents the ** array
// and clear the binfile pointer
	free(cpu);
	fclose(saveCPUs);

	printf("Processors sent to storage!\n");
	PAUSE;

	return;
} // end savePhones
void searchCPUs(PROCESSOR **cpu, int eSize) {
	int c, first, last, middle;
	int bottom = eSize - 1;
	char swapped = 'Y';
	char desiredSKU[5];

	PROCESSOR **temp = calloc(eSize, sizeof(PROCESSOR*)); //BASICALLY THE SIZE OF A POINTER, we are duplicating the addresses not the items
	if (temp == NULL) {
		printf("You need to download more RAM\nOut of memory!\n");
		exit(-1);
	}// end if

	PROCESSOR **tempNode = calloc(1, sizeof(PROCESSOR*));
	if (tempNode == NULL) {
		printf("Out of memory!\n");
		exit(-1);
	}// end if

	//copy array
	for (int i = 0; i < eSize; i++)
		temp[i] = cpu[i];

	do {
		swapped = 'N';
		for (int i = 0; i < bottom; i++) {
			if ((strcmp(temp[i]->processorBrand, temp[i + 1]->processorBrand) > 0)) {
				tempNode = temp[i];
				temp[i] = temp[i + 1];
				temp[i + 1] = tempNode;
				swapped = 'Y';
			}//end if
			else if ((strcmp(temp[i]->processorBrand, temp[i + 1]->processorBrand) == 0)) {
				if (strcmp(temp[i]->processorName, temp[i + 1]->processorName) > 0) {
					tempNode = temp[i];
					temp[i] = temp[i + 1];
					temp[i + 1] = tempNode;
					swapped = 'Y';
				}
				else if (strcmp(temp[i]->processorName, temp[i + 1]->processorName) == 0) {
					if ((strcmp(temp[i]->SKU, temp[i + 1]->SKU) > 0)) {
						tempNode = temp[i];
						temp[i] = temp[i + 1];
						temp[i + 1] = tempNode;
						swapped = 'Y';
					}//end if
				}
			}//end ifelse
		}
	} while (swapped == 'Y');

	stringValidator(eSize, sizeof(desiredSKU), desiredSKU, "SKU");
	first = 0;
	last = eSize - 1;
	middle = (first + last) / 2;

	while (first <= last) {
		if (strcmp(temp[middle]->SKU, desiredSKU) < 0)
			first = middle + 1;
		else if (strcmp(temp[middle]->SKU, desiredSKU) == 0){
			printf("%s found!\n", desiredSKU);
			displaySearchResult(cpu, middle);
			break;
		}
		else
			last = middle - 1;

		middle = (first + last) / 2;
	}
	if (first > last)
		printf("Not found! %s isn't in stock.\n", desiredSKU);
}//end search

void displaySearchResult(PROCESSOR **cpu, int i) {
	//define lengths
	char coreThread[10];
	sprintf(coreThread, "%d / %d", 0, 0);
	char clockString[12];
	sprintf(clockString, "%.1f/%.1f ghz", 0, 0);
	char TDPString[7];
	sprintf(TDPString, "%i", 0);
	char quantityString[12];
	sprintf(quantityString, "%i", 0);


	//TOP DISPLAY ensure space added after each printf
	printf("\n# | Brand");
	formatCategoryDisplay(sizeof("Brand"), 8);
	//name
	printf(" | ");
	printf("Name");
	formatCategoryDisplay(sizeof("Name"), 12);
	//SKU
	printf(" | ");
	printf("SKU");
	formatCategoryDisplay(sizeof("SKU"), 6);
	//core/thread
	printf(" | ");
	printf("Cores/Threads");
	formatCategoryDisplay(sizeof("Core/Threads"), 12);
	//clockString
	printf(" | ");
	printf("Base/Boost Clocks");
	formatCategoryDisplay(sizeof("Base/Boost Clocks"), 10);
	//TDP
	printf(" | ");
	printf("TDP/W");
	formatCategoryDisplay(strlen("TDP/W"), strlen("TDP/W"));
	//Count
	printf(" | ");
	printf("Quantity");
	formatCategoryDisplay(strlen("Quantity"), 8);
	//price
	printf(" | ");
	printf("Price");
	formatCategoryDisplay(strlen("Price"), 8);
	printf("\n");

	//thing
	for (int i = 0; i < 100; i++) {
		printf("=");
	}

		//string formatters
		memset(coreThread, 0, strlen(coreThread));
		memset(clockString, 0, strlen(clockString));
		memset(TDPString, 0, strlen(TDPString));
		memset(quantityString, 0, strlen(quantityString));

		sprintf(coreThread, "%d / %d", cpu[i]->specs.coreCount, cpu[i]->specs.threadCount);
		sprintf(clockString, "%.1f/%.1f ghz", cpu[i]->specs.clock, cpu[i]->specs.boostClock);
		sprintf(TDPString, "%i", cpu[i]->specs.wattageTDP);
		sprintf(quantityString, "%i", cpu[i]->count);

		//BOTTOM DISPLAY
		//number
		printf("\n%i", i + 1);
		//brand
		printf(" | ");
		printf("%s", cpu[i]->processorBrand);
		formatCPUdisplay(strlen(cpu[i]->processorBrand), sizeof(cpu[i]->processorBrand));
		//name'
		printf(" | ");
		printf("%s", cpu[i]->processorName);
		formatCPUdisplay(strlen(cpu[i]->processorName), sizeof(cpu[i]->processorName));
		//SKY
		printf(" | ");
		printf("%s", cpu[i]->SKU);
		formatCategoryDisplay(strlen(cpu[i]->SKU), sizeof(cpu[i]->SKU));
		//core/thread
		printf(" | ");
		printf("%s", coreThread);
		formatCPUdisplay(strlen(coreThread), strlen("Cores/Threads"));
		//clockString
		printf(" | ");
		printf("%s", clockString);
		formatCPUdisplay(strlen(clockString), strlen("Base/Boost Clocks"));
		//TDP
		printf(" | ");
		printf("%i", cpu[i]->specs.wattageTDP);
		formatCPUdisplay(strlen(TDPString), strlen("TDP/W"));
		//Quantitiy
		printf(" | ");
		printf("%i", cpu[i]->count);
		formatCPUdisplay(strlen(quantityString), strlen("Quantity"));
		//Quantitiy
		printf(" | ");
		printf("$%.2f", cpu[i]->processorPrice);
		formatCPUdisplay(strlen(quantityString), strlen("Price"));
	printf("\n");
	PAUSE;

}
void sortProcessors(PROCESSOR **cpu, int eSize, char flag) {
	int bottom = eSize - 1;
	char swapped;
	//temp storage
	PROCESSOR **temp = calloc(eSize, sizeof(PROCESSOR*)); //BASICALLY THE SIZE OF A POINTER, we are duplicating the addresses not the items
	if (temp == NULL) {
		printf("You need to download more RAM\nOut of memory!\n");
		exit(-1);
	}// end if

	PROCESSOR **tempNode = calloc(1, sizeof(PROCESSOR*));
	if (tempNode == NULL) {
		printf("Out of memory!\n");
		exit(-1);
	}// end if

	//copy array
	for (int i = 0; i < eSize; i++)
		temp[i] = cpu[i];


	//sort by Brand
	if (flag == 'B') {
	do {
		swapped = 'N';
		for (int i = 0; i < bottom; i++) {
				if (strcmp(temp[i]->processorBrand, temp[i + 1]->processorBrand) > 0) {
					printf("SWAPPING");
					tempNode = temp[i];
					temp[i] = temp[i + 1];
					temp[i + 1] = tempNode;
					swapped = 'Y';
				}//end if
			}//end flag
		} while (swapped == 'Y');
}//end flag

	if (flag == 'O') {
		do {
			swapped = 'N';
			for (int i = 0; i < bottom; i++) {
				printf("in first");
				if ((strcmp(temp[i]->processorBrand, temp[i + 1]->processorBrand) > 0)) {
					printf("swap 1");
					tempNode = temp[i];
					temp[i] = temp[i + 1];
					temp[i + 1] = tempNode;
					swapped = 'Y';
				}//end if
				else if ((strcmp(temp[i]->processorBrand, temp[i + 1]->processorBrand) == 0)) {
					printf("in brand");
					if (strcmp(temp[i]->processorName, temp[i + 1]->processorName) > 0) {
						printf("swap2");
						tempNode = temp[i];
						temp[i] = temp[i + 1];
						temp[i + 1] = tempNode;
						swapped = 'Y';
					}
					else if (strcmp(temp[i]->processorName, temp[i + 1]->processorName) == 0) {
						if ((strcmp(temp[i]->SKU, temp[i + 1]->SKU) > 0)) {
							printf("swap2");
							tempNode = temp[i];
							temp[i] = temp[i + 1];
							temp[i + 1] = tempNode;
							swapped = 'Y';
						}//end if
					}
				}//end ifelse
			}
		} while (swapped == 'Y');
	}//end if O Flag
	if (flag == 'P') {
		do {
			swapped = 'N';
			for (int i = 0; i < bottom; i++) {
					if (temp[i]->processorPrice > temp[i + 1]->processorPrice) {
						//by price
						tempNode = temp[i];
						temp[i] = temp[i + 1];
						temp[i + 1] = tempNode;
						swapped = 'Y';
					}//end if
			}//end loop
		} while (swapped == 'Y');
	}//end flag
	displayCPUTypes(temp, &eSize);
}//end sort processors
void stringValidator(int *eSize, int lengthDesired, PROCESSOR *temp, char msg[]) {
	//temp stores the char array inside the struct that we actually want to change
	char errorCheck[30]; // no need to free as it only exists in the scope of this function
	do {
		errorCheck[0] = '\0'; //strlen wont look past a null
		printf("Enter the %s.\n", msg);
		scanf("%30s[a-zA-Z]", &errorCheck);
		FLUSH;
		if (strlen(errorCheck) <= lengthDesired) {
			sentenceCase(errorCheck); // in header class
			strcpy(temp, errorCheck);
		}
		else
			printf("Please limit the brand to %i characters", lengthDesired);
	} while (strlen(errorCheck) > lengthDesired);
}//end stringValidator
