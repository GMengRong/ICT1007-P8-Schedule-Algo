// C program for Highest Response Ratio Next (HRRN) Scheduling
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct fileData // Data read from file
{
    int arrivaltimes[255];
    int bursttimes[255];
    int processno;
};

struct process {
	int at, bt, ct, wt, tt;
	int completed;
	float ntt;
};

int n; // No of testcases to run
float *** test_results; // float * 3D array containing all the test results []

void mem_aloc() {
	test_results = (float ***)malloc(4*sizeof(float*)); // Allocating memory to the X-axis of the array
		if (test_results == NULL) // Check if the memory was allocated
		{
			fprintf(stderr, "Out of memory");
			exit(0);
		}

		for(int r = 0; r < 4; r++){		
			test_results[r] = (float**)malloc(n*sizeof(float*)); // Allocating memory to the rows
			if (test_results[r] == NULL) // Check if the memory was allocated
			{
				fprintf(stderr, "Out of memory");
				exit(0);
			}

			for(int c = 0; c < n; c++){
				test_results[r][c] = (float*)malloc(4*sizeof(float));
				if (test_results[r][c] == NULL)
				{
					fprintf(stderr, "Out of memory");
					exit(0);
				}
			}
		}
}

struct fileData readFile(char *FileName)
{
	// printf("readfile function filename = %s\n", FileName);// TODELETE
    FILE *testcase;

    testcase = fopen(FileName, "r");

    int i = 0;
    int arrivaltimes[255];
    int bursttimes[255];

    if (testcase == NULL)
    {
        printf("Error in opening testcase file\n");
        fclose(testcase);
        exit(0);
    }

    while (fscanf(testcase, "%d %d", &arrivaltimes[i], &bursttimes[i]) != EOF)
    {
        i++;
    }

    arrivaltimes[i] = '\0';
    bursttimes[i] = '\0';

    fclose(testcase);

    struct fileData processdata = {.processno = i};
    memcpy(processdata.arrivaltimes, arrivaltimes, sizeof arrivaltimes);
    memcpy(processdata.bursttimes, bursttimes, sizeof bursttimes);

    return processdata;
}

// Sorting Processes by Arrival Time
void sortByArrival(struct process p[])
{
	struct process temp;
	int i, j;

	// Selection Sort applied
	for (i = 0; i < n - 1; i++) {
		for (j = i + 1; j < n; j++) {

			// Check for lesser arrival time
			if (p[i].at > p[j].at) {

				// Swap earlier process to front
				temp = p[i];
				p[i] = p[j];
				p[j] = temp;
			}
		}
	}
}

// Array to clone the processes for each algorithm
void cloneArray(struct process original[], struct process cloned[], int size){
	// Iterate through all the elements in the original array
	for (int i = 0; i < size; i++){
		cloned[i] = original[i];
	}
}

void hrrn(struct process p[], int numberOfProcesses, int sum_bt, int testNo){
	float avgwt = 0, avgtt = 0; // Average waiting time and average turnaround time
	struct process p_cloned[8];
	cloneArray(p, p_cloned, numberOfProcesses);
	for (int x = 0; x<numberOfProcesses; x++){
		printf("p_clone[%d].at == %d\n", x, p_cloned[x].at);
	}

	printf("\nName\tArrival Time\tBurst Time\tWaiting Time");
	printf("\tTurnAround Time\t Normalized TT");
	for (int t = p_cloned[0].at; t < sum_bt;){
		float hrr = -9999; // Set lower limit to response ratio
		float temp; // Response ratio variable
		int loc; // Variable to store next process selected

		for(int i = 0; i < numberOfProcesses; i++){
			// Checking if process has arrived and is Incomplete
			if (p_cloned[i].at <= t && p_cloned[i].completed != 1) {

				// Calculating Response Ratio
				temp = (float)(p_cloned[i].bt + (t - p_cloned[i].at)) / (float)p_cloned[i].bt;
				// printf("\n temp = %f\n", temp);
				// printf("\n\t:%d + (%d - %d)/%d = %f\n",  p_cloned[i].bt, t, p_cloned[i].at, p_cloned[i].bt, temp);

				// Checking for Highest Response Ratio
				if (hrr < temp) {

					// Storing Response Ratio
					hrr = temp;
					// printf("%f", hrr);

					// Storing Location
					loc = i;
				}
				else if(hrr == temp){
					if (p_cloned[loc].bt > p_cloned[i].bt){
						loc = i;
					}
				}
			}
		}

		// Updating time value
		t += p_cloned[loc].bt;

		// Calculation of waiting time
		p_cloned[loc].wt = t - p_cloned[loc].at - p_cloned[loc].bt;

		// Calculation of Turn Around Time
		p_cloned[loc].tt = t - p_cloned[loc].at;

		// Sum Turn Around Time for average
		avgtt += p_cloned[loc].tt;

		// Calculation of Normalized Turn Around Time
		p_cloned[loc].ntt = ((float)p_cloned[loc].tt / p_cloned[loc].bt);

		// Updating Completion Status
		p_cloned[loc].completed = 1;

		// Sum Waiting Time for average
		avgwt += p_cloned[loc].wt;
		printf("\n\t\t%d\t\t", p_cloned[loc].at);
		printf("%d\t\t%d\t\t", p_cloned[loc].bt, p_cloned[loc].wt);
		printf("%d\t\t%f", p_cloned[loc].tt, p_cloned[loc].ntt);
	}

	// Calculation for max wait time and max turnaround time 
	float maxwaitTime = 0.0, maxturnaroundTime = 0.0;
    
    for (int j = 0; j < numberOfProcesses; j++)
	{
		if (p_cloned[j].wt > maxwaitTime){
			maxwaitTime = p_cloned[j].wt;
		}
		if (p_cloned[j].tt > maxturnaroundTime) {
			maxturnaroundTime = p_cloned[j].tt;
		}
	}
	avgwt /= numberOfProcesses;
	avgtt /= numberOfProcesses;
	// printf("\nbut before that here are the stats: awt = %f, mwt = %f, att = %f, mtt = %f\n", avgwt, maxturnaroundTime, avgtt, maxturnaroundTime);
	// printf("\ninside hrrn testcase results: %f,%f,%f,%f\n", testcase_results[0],testcase_results[1],testcase_results[2],testcase_results[3]);
	// Population of the test_results 3d array
	test_results[0][testNo-1][0] = avgwt;
	test_results[0][testNo-1][1] = maxwaitTime;
	test_results[0][testNo-1][2] = avgtt;
	test_results[0][testNo-1][3] = maxturnaroundTime;
}

void loop(int n) {

	mem_aloc(); // allocate memory for the testcases
	// printf("sizeof test reults [0][0] is %d, sizeof int* is %d" ,sizeof(test_results[0][0]), sizeof(int*)); // TODELETE
	for(int testNo = 1; testNo<n+1; testNo++) {
		struct process p[8]; // Initialise the struct array containing all the processes

		int sum_bt = 0;
		char filename[28]; // Initialise filename to be read;
		sprintf(filename,"./testcases/testcase%d.txt", testNo); // Format the file name and save value to the filename variable
		// printf("filename = %s\n",filename); // TODELETE
		struct fileData file_data = readFile(filename); // Reading testcase file to get the file data
		int no_of_processes = file_data.processno;

		// Initializing the structure variables
		for (int i = 0; i < no_of_processes; i++) {
			p[i].at = file_data.arrivaltimes[i];
			p[i].bt = file_data.bursttimes[i];

			// Variable for Completion status
			// Pending = 0
			// Completed = 1
			p[i].completed = 0;

			// Variable for sum of all Burst Times
			sum_bt += p[i].bt;
		}

		sortByArrival(p);
		
		hrrn(p, no_of_processes, sum_bt, testNo);
		printf("hrrn test results = %f,%f,%f,%f", test_results[0][testNo-1][0],test_results[0][testNo-1][1],test_results[0][testNo-1][2],test_results[0][testNo-1][3]);
		// for(int k = 0; k < no_of_processes; k++){
			
		// 	printf("Process, at = %d, bt = %d, wt = %d, tt=%d, ntt=%f\n",p[k].at,p[k].bt,p[k].wt,p[k].tt,p[k].ntt);
		// }
		
	}
	// test_results;
}

void main() {
	printf("How many testcases to run? (Please create testcases first) ");
	scanf("%d",&n);
	loop(n);
	// test_results[0][0] = 4;
	// test_results[0][1] = 3;
	// test_results[0][2] = 2;
	// test_results[0][3] = 1;
	// test_results[1][0] = 8;
	// test_results[1][1] = 7;
	// test_results[1][2] = 6;
	// test_results[1][3] = 5;

	// for(int r = 0; r < n; r++){
	// 	for(int c = 0; c < 4; c++){
	// 		printf("row %d , column %d == %d\n", r,c,test_results[r][c]);
	// 	}
	// }
	// loop(100);
}