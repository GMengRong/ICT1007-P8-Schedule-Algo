// C program for Highest Response Ratio Next (HRRN) Scheduling
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define processesInFile 8 // Number of processes in each test case
#define ALGOCOUNT 4
// int TESTCOUNT = 10;

struct fileData // Data read from file
{
    int arrivaltimes[255]; // Array containing all the arrival times in the file
    int bursttimes[255]; // Array containing all the burst times in the file
    int processno; // Number of processes in the file
};

struct process // Process 
{
	int at, bt, rbt, wt, tt; // arrival time, burst time, remaining burst time, wait time, turnaround time
	int completed; // Int value to indicate if the process is completed or not 
	float ntt; // Normalised turnaround time 
};

int n; // No of testcases to run
float *** test_results; // float * 3D array containing all the test results
// test_results[algorithm][test number][Matrix]
// algorithm 0 = HRRN, 1 = HRRN+1, 2 = HRRN with Preemption, 3 = HRRN + 1 with preemption
// matrix 0 = Average wait time, 1 = max wait time, 2 = average turnaround time, 3 = max turnaround time

// void writeToCsv(float pointerscansuckmydick[ALGOCOUNT][TESTCOUNT][4]){
void writeToCsv(float *** pointerscansuckmydick, int testcount){
    FILE *fptr;
    fptr = fopen("hrrnresults.csv", "w");

    if(fptr == NULL){
        printf("Shit broke im sorry\n");
        exit(1);
    }

    fprintf(fptr, "Testcase Number, HRRN,,,, HRRN+1,,,, HRRN Preempt,,,, HRRM Preempt+1,,,\n");
    fprintf(fptr,", AWT, MWT, ATT, MTT, AWT, MWT, ATT, MTT, AWT, MWT, ATT, MTT, AWT, MWT, ATT, MTT\n");

    for (int i = 0; i < testcount; i++){
        fprintf(fptr, "%d,",i+1);
        for (int j = 0; j < ALGOCOUNT; j++){
            fprintf(fptr,"%f,",pointerscansuckmydick[j][i][0]);
            fprintf(fptr,"%f,",pointerscansuckmydick[j][i][1]);
            fprintf(fptr,"%f,",pointerscansuckmydick[j][i][2]);
            fprintf(fptr,"%f,",pointerscansuckmydick[j][i][3]);
        }
        fprintf(fptr,"\n");
    }
    
    fclose(fptr);
    printf("Done\n");
    
}

void mem_aloc() {
	test_results = (float ***)malloc(4*sizeof(float*)); // Allocating memory to the blocks of the array
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
				test_results[r][c] = (float*)malloc(4*sizeof(float)); // Allocating memory to the columns
				if (test_results[r][c] == NULL) // Check if the memory was allocated
				{
					fprintf(stderr, "Out of memory");
					exit(0);
				}
			}
		}
}

struct fileData readFile(char *FileName)
{
	printf("readfile function filename = %s\n", FileName);// TODELETE
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
		printf("item%d arrival time of %d, bursttime of %d\n", i, arrivaltimes[i], bursttimes[i]);
        i++;
    }

    arrivaltimes[i] = '\0';
    bursttimes[i] = '\0';

    fclose(testcase);

    struct fileData file_data = {.processno = i};
    memcpy(file_data.arrivaltimes, arrivaltimes, sizeof(arrivaltimes));
    memcpy(file_data.bursttimes, bursttimes, sizeof(bursttimes));

    return file_data;
}

// Sorting Processes by Arrival Time
void sortByArrival(struct process p[], int no_of_processes)
{
	struct process temp;
	int i, j;

	// Selection Sort applied
	for (i = 0; i < no_of_processes - 1; i++) {
		for (j = i + 1; j < no_of_processes; j++) {

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

// HRRN process scheduling algorithm
void hrrn(struct process p[], int numberOfProcesses, int sum_bt, int testNo, int plusOne){
	if (plusOne){
		printf("\ntestcase %d In HRRN + 1 scheduler\n", testNo);
	} 
	else {
		printf("\ntestcase %d In HRRN scheduler\n",testNo);
	}
	
	float avgwt = 0, avgtt = 0; // Average waiting time and average turnaround time
	struct process p_cloned[processesInFile];
	cloneArray(p, p_cloned, numberOfProcesses);

	for (int x = 0; x<numberOfProcesses; x++){
		printf("p_clone[%d].at == %d\n", x, p_cloned[x].at);
		printf("p_clone[%d].bt == %d\n", x, p_cloned[x].bt);
		printf("p_clone[%d].wt == %d\n", x, p_cloned[x].wt);
	}

	// printf("\nName\tArrival Time\tBurst Time\tWaiting Time");
	// printf("\tTurnAround Time\t Normalized TT");
	for (int t = p_cloned[0].at; t < sum_bt;){
		float hrr = -9999; // Set lower limit to response ratio
		float temp; // Response ratio variable
		int loc; // Variable to store next process selected

		for(int i = 0; i < numberOfProcesses; i++){
			// Checking if process has arrived and is Incomplete
			if (p_cloned[i].at <= t && p_cloned[i].completed != 1) {

				// Calculating Response Ratio
				temp = (float)(p_cloned[i].bt + (t - p_cloned[i].at) + plusOne) / (float)p_cloned[i].bt;
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
		// printf("\n\t\t%d\t\t", p_cloned[loc].at);
		// printf("%d\t\t%d\t\t", p_cloned[loc].bt, p_cloned[loc].wt);
		// printf("%d\t\t%f", p_cloned[loc].tt, p_cloned[loc].ntt);
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
	test_results[0+plusOne][testNo-1][0] = avgwt;	
	test_results[0+plusOne][testNo-1][1] = maxwaitTime;
	test_results[0+plusOne][testNo-1][2] = avgtt;
	test_results[0+plusOne][testNo-1][3] = maxturnaroundTime;
}

// HRRN with preemption process scheduling algorithm
void hrrnwithpreemption (struct process p[], int numberOfProcesses, int sum_bt, int testNo, int plusOne) {
	if (plusOne){
		printf("\nTestcase %d In HRRN + 1 with preemption scheduler\n", testNo);
	} 
	else {
		printf("\nTestcase %d In HRRN with preemption scheduler\n",testNo);
	}
	struct process p_cloned[processesInFile];
	cloneArray(p, p_cloned, numberOfProcesses);
	for (int x = 0; x<numberOfProcesses; x++){
		printf("p_clone[%d].at == %d\n", x, p_cloned[x].at);
		printf("p_clone[%d].bt == %d\n", x, p_cloned[x].bt);
		printf("p_clone[%d].wt == %d\n", x, p_cloned[x].wt);
	}
	
	// Set lower limit to response ratio
	float hrr = -9999;

	// Response Ratio Variable
	float temp;

	// Variable to store next process selected and previous process in the event of interrupt
	int currentprocess = 0, previousprocess = 0;

	// Variables for current time and ime quantum
	int currenttime = 0;
	int quantum = 0;
	int runTime = 0;
	float avgwaitTime = 0, avgturnaroundTime = 0, maxwaitTime = 0, maxturnaroundTime = 0, timedelay = 0;

	// To track number of completed processes
	int completedprocesses = 0;

	// To track total wait time of all ready processes
	int queueTotalWaitTime = 0;

	// To track number of ready processes in queue
	int queueCount = 0;

	// To track wt(running proc)/len(waiting procs) + bt(remaining)
	double currentProcScore = 0;

	// Interrupt boolean to determine when to interrupt process
	bool interrupt;

	// While loop to perform running of all processes.
	while (completedprocesses < numberOfProcesses)
	{
		// Calc total wait time of all queued processes
		queueTotalWaitTime = 0;
		queueCount = 0;
		runTime++;
		for (int y = 0; y < numberOfProcesses; y++)
		{
			// If waiting, add to queueTotalWaitTime
			if (p_cloned[y].at <= runTime && p_cloned[y].completed != 1)
			{
				if (y != currentprocess)
				{
					queueTotalWaitTime += p_cloned[y].wt+1;
					queueCount++;
				}
			}
		}
		queueTotalWaitTime--;
		runTime--;

		// This is debug code, leaving here in case need to check more shid
		// printf("Currentprocess: %d\n", currentprocess);
		// if (currentprocess >= 0){
		// 	printf("WT: %d\n", p_cloned[currentprocess].wt);
		// 	printf("QueueCount: %d\n", queueCount);
		// 	printf("BTLeft: %d\n", p_cloned[currentprocess].rbt);
		// }

		// Do the actual check. If fail, process as usual
		// If succeed and ready proc wt is more, which to swap to?
		if (queueCount != 0)
		{ // If queue == 0, a prog is running with no queue

			if (queueTotalWaitTime > p_cloned[currentprocess].rbt && p_cloned[currentprocess].completed != 1 && timedelay < 1 && queueCount > 1)
			{
				previousprocess = currentprocess;
				//printf("%d : %d\n", queueTotalWaitTime,p_cloned[currentprocess].remainingBurstTime);
				interrupt = true;
				// If interrupt, set a delay for the program.
				timedelay = queueTotalWaitTime;
			}

			// currentProcScore = (p_cloned[currentprocess].waitTime / queueCount) + p_cloned[currentprocess].remainingBurstTime;
			// if ((queueTotalWaitTime > currentProcScore) || p_cloned[currentprocess].completed == 1){
			// swippity swappity to which one???
			// This is probably supposed to go outside of the HRR

			// To reset response ratio for each iteration.
			hrr = -9999;
			// To determine which process to run (HRRN)
			if (interrupt || p_cloned[currentprocess].completed == 1)
			{
				for (int i = 0; i < numberOfProcesses; i++)
				{
					// To skip the previous process if interrupt was triggered.
					if (interrupt && i == previousprocess)
					{
						i++;
					}

					// Checking if process has arrived and is Incomplete
					if (p_cloned[i].at <= runTime && p_cloned[i].completed != 1)
					{
						// Calculating Response Ratio
						// temp = (float)(p_cloned[i].burstTime + (runTime - p_cloned[i].arrivalTime)) / (float)p_cloned[i].burstTime;
						temp = (float)(p_cloned[i].rbt + (runTime - p_cloned[i].at) + plusOne) / (float)p_cloned[i].rbt;

						// Checking for Highest Response Ratio
						if (hrr < temp)
						{
							// Storing Response Ratio
							hrr = temp;

							// Storing Location
							currentprocess = i;
						}
						else if (hrr == temp)
						{
							// if (p_cloned[currentprocess].burstTime > p_cloned[i].burstTime)
							if (p_cloned[currentprocess].bt > p_cloned[i].rbt)
							{
								currentprocess = i;
							}
						}
					}
				}
			}
		}

		// Process to run has been selected. Increase wait time and turnaround time of all ready processes.
		// printf("%c",p_cloned[currentprocess].name);
		for (int j = 0; j < numberOfProcesses; j++)
		{
			if (p_cloned[j].at <= runTime && p_cloned[j].completed != 1)
			{
				// Check if its not the current process
				if (j != currentprocess)
				{
					p_cloned[j].wt++;
					p_cloned[j].tt++;
				}
			}
		}
		// To decrement the remaining burst time of current process and increase turnaround time.
		p_cloned[currentprocess].rbt--;
		p_cloned[currentprocess].tt++;

		// If process has 0 remaining burst time, count as completed.
		if (p_cloned[currentprocess].rbt == 0)
		{
			p_cloned[currentprocess].completed = 1;
			completedprocesses++;
		}
		// Increase the runtime by 1, decrease the delay by one.
		interrupt = false;
		timedelay--;
		runTime++;
	}

	// Print statement logic
	// printf("\nName\tArrival Time\tBurst Time\tWaiting Time");
	// printf("\tTurnAround Time");

	for (int j = 0; j < numberOfProcesses; j++)
	{
		avgwaitTime += p_cloned[j].wt;
		avgturnaroundTime += p_cloned[j].tt;
		// printf("\n\t\t%d\t\t", p_cloned[j].at);
		// printf("%d\t\t%d\t\t", p_cloned[j].bt, p_cloned[j].wt);
		// printf("%d", p_cloned[j].tt);
		if (p_cloned[j].wt > maxwaitTime)
		{
			maxwaitTime = p_cloned[j].wt;
		}
		if (p_cloned[j].tt > maxturnaroundTime)
		{
			maxturnaroundTime = p_cloned[j].tt;
		}
	}

	// printf("\nAverage waiting time:%f\n", avgwaitTime / numberOfProcesses);
	// printf("Maximum waiting time:%f\n", maxwaitTime);
	// printf("Average Turn Around time:%f\n", avgturnaroundTime / numberOfProcesses);
	// printf("Maximum Turn Around time:%f\n", maxturnaroundTime);
	printf("saving results\n");
	
	test_results[2+plusOne][testNo-1][0] = avgwaitTime / numberOfProcesses;
	test_results[2+plusOne][testNo-1][1] = maxwaitTime;
	test_results[2+plusOne][testNo-1][2] = avgturnaroundTime / numberOfProcesses;
	test_results[2+plusOne][testNo-1][3] = maxturnaroundTime;
}

void loop(int n) {
	// printf("sizeof test reults [0][0] is %d, sizeof int* is %d" ,sizeof(test_results[0][0]), sizeof(int*)); // TODELETE
	for(int testNo = 1; testNo<n+1; testNo++) {
		struct process p[processesInFile]; // Initialise the struct array containing all the processes

		int sum_bt = 0;
		char filename[45]; // Initialise filename to be read;
		sprintf(filename,"./testcases/testcase%d.txt", testNo); // Format the file name and save value to the filename variable
		// printf("filename = %s\n",filename); // TODELETE
		struct fileData file_data = readFile(filename); // Reading testcase file to get the file data
		int no_of_processes = file_data.processno;
		// printf("no_of_processes = %d\n", no_of_processes);
		// Initializing the structure variables
		for (int i = 0; i < no_of_processes; i++) {
			p[i].at = file_data.arrivaltimes[i];
			p[i].bt = file_data.bursttimes[i];
			p[i].wt = 0;
			p[i].tt = 0;
			p[i].rbt = file_data.bursttimes[i];

			// Variable for Completion status
			// Pending = 0
			// Completed = 1
			p[i].completed = 0;

			// Variable for sum of all Burst Times
			sum_bt += p[i].bt;
		}

		for (int x = 0; x<no_of_processes; x++){
			printf("official p[%d].at == %d\n", x, p[x].at);
			printf("official p[%d].bt == %d\n", x, p[x].bt);
			printf("official p[%d].wt == %d\n", x, p[x].wt);
		}

		sortByArrival(p, no_of_processes);
		
		for (int x = 0; x<no_of_processes; x++){
			printf("official p[%d].at == %d\n", x, p[x].at);
			printf("official p[%d].bt == %d\n", x, p[x].bt);
			printf("official p[%d].wt == %d\n", x, p[x].wt);
		}

		hrrn(p, no_of_processes, sum_bt, testNo, 0);
		hrrn(p, no_of_processes, sum_bt, testNo, 1);
		hrrnwithpreemption(p, no_of_processes, sum_bt, testNo, 0);
		hrrnwithpreemption(p, no_of_processes, sum_bt, testNo, 1);

		printf("\nResults for testcase %d:\n", testNo);
		printf("hrrn test results = %f,%f,%f,%f", test_results[0][testNo-1][0],test_results[0][testNo-1][1],test_results[0][testNo-1][2],test_results[0][testNo-1][3]);
		printf("\nhrrn+1 test results = %f,%f,%f,%f", test_results[1][testNo-1][0],test_results[1][testNo-1][1],test_results[1][testNo-1][2],test_results[1][testNo-1][3]);
		printf("\nhrrn with Preemption test results = %f,%f,%f,%f", test_results[2][testNo-1][0],test_results[2][testNo-1][1],test_results[2][testNo-1][2],test_results[2][testNo-1][3]);
		printf("\nhrrn+1 with Preemption test results = %f,%f,%f,%f\n", test_results[3][testNo-1][0],test_results[3][testNo-1][1],test_results[3][testNo-1][2],test_results[3][testNo-1][3]);


		// for(int k = 0; k < no_of_processes; k++){
			
		// 	printf("Process, at = %d, bt = %d, wt = %d, tt=%d, ntt=%f\n",p[k].at,p[k].bt,p[k].wt,p[k].tt,p[k].ntt);
		// }
		
	}
	// test_results;
}

void main() {
	printf("How many testcases to run? (Please create testcases first) ");
	scanf("%d",&n);
	mem_aloc(); // allocate memory for the testcases
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
	writeToCsv(test_results,n);
}