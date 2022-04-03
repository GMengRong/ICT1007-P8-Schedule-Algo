// C program for Preemptive HRRN+1 Scheduling
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Struct which will contain all process data. The processes will be stored into an array "p".
struct process
{
	int pid;
	int arrivalTime, burstTime, remainingBurstTime, waitTime, turnaroundTime;
	int completed;
} p[255];

// Sorting Processes by Arrival Time
void sortByArrival(int numberOfProcess)
{
	struct process temp;
	int i, j;

	// Selection Sort applied
	for (i = 0; i < numberOfProcess - 1; i++)
	{
		for (j = i + 1; j < numberOfProcess; j++)
		{

			// Check for lesser arrival time
			if (p[i].arrivalTime > p[j].arrivalTime)
			{

				// Swap earlier process to front
				temp = p[i];
				p[i] = p[j];
				p[j] = temp;
			}
		}
	}
}

// The struct for the filereader function, which contains two arrays for the burst and arrival times.
struct ProcessesData
{
	int arrivaltimes[255];
	int bursttimes[255];
	int processno;
};

// File reader function to store all data into a list of arrival times and burst times.
struct ProcessesData readFile(char *FileName)
{

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

	struct ProcessesData processdata = {.processno = i};
	memcpy(processdata.arrivaltimes, arrivaltimes, sizeof arrivaltimes);
	memcpy(processdata.bursttimes, bursttimes, sizeof bursttimes);

	return processdata;
}

float* hrrnwithpreemption (struct process p[], int numberOfProcesses) {
	static float resultarray[4];
	// Set lower limit to response ratio
	float hrr = -9999;

	// Response Ratio Variable
	float temp;
	
	bool interrupt;
	
	float avgwaitTime = 0, avgturnaroundTime = 0, maxwaitTime = 0, maxturnaroundTime = 0, timedelay = 0;

	// Variable to store next process selected and previous process in the event of interrupt
	int currentprocess = 0, previousprocess = 0, runTime = 0;

	// Variables for current time and ime quantum
	int currenttime = 0;
	int quantum = 0;

	// To track number of completed processes
	int completedprocesses = 0;

	// To track total wait time of all ready processes
	int queueTotalWaitTime = 0;

	// To track number of ready processes in queue
	int queueCount = 0;

	// To track wt(running proc)/len(waiting procs) + bt(remaining)
	double currentProcScore = 0;

	// While loop to perform running of all processes.
	while (completedprocesses < numberOfProcesses)
	{
		// Calc total wait time of all queued processes
		queueTotalWaitTime = 0;
		queueCount = 0;
		runTime++;
		for (int i = 0; i < numberOfProcesses; i++)
		{
			// If waiting, add to queueTotalWaitTime
			if (p[i].arrivalTime <= runTime && p[i].completed != 1)
			{
				if (i != currentprocess)
				{
					queueTotalWaitTime += p[i].waitTime+1;
					queueCount++;
				}
			}
		}
		queueTotalWaitTime--;
		runTime--;
		if (queueCount != 0)
		{ // If queue == 0, a prog is running with no queue

			if (queueTotalWaitTime > p[currentprocess].remainingBurstTime && p[currentprocess].completed != 1 && timedelay < 1 && queueCount > 1)
			{
				// Store the interrupted process to ensure that it does not get run again.
				previousprocess = currentprocess;
				interrupt = true;
				// If interrupt, set a delay for the program.
				timedelay = queueTotalWaitTime;
			}

			// To reset response ratio for each iteration.
			hrr = -9999;
			int i = 0;
			// To determine which process to run (HRRN)
			if (interrupt || p[currentprocess].completed == 1)
			{
				for (i = 0; i < numberOfProcesses; i++)
				{
					// To skip the previous process if interrupt was triggered.
					if (interrupt && i == previousprocess)
					{
						i++;
					}

					// Checking if process has arrived and is Incomplete
					if (p[i].arrivalTime <= runTime && p[i].completed != 1)
					{
						// Calculating Response Ratio
						temp = (float)(1 + p[i].remainingBurstTime + (runTime - p[i].arrivalTime)) / (float)p[i].remainingBurstTime;

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
							if (p[currentprocess].burstTime > p[i].remainingBurstTime)
							{
								currentprocess = i;
							}
						}
					}
				}
			}
		}

		// Process to run has been selected. Increase wait time and turnaround time of all ready processes.
		for (int i = 0; i < numberOfProcesses; i++)
		{
			if (p[i].arrivalTime <= runTime && p[i].completed != 1)
			{
				// Check if its not the current process
				if (i != currentprocess)
				{
					p[i].waitTime++;
					p[i].turnaroundTime++;
				}
			}
		}
		// To decrement the remaining burst time of current process and increase turnaround time.
		p[currentprocess].remainingBurstTime--;
		p[currentprocess].turnaroundTime++;

		// If process has 0 remaining burst time, count as completed.
		if (p[currentprocess].remainingBurstTime == 0)
		{
			p[currentprocess].completed = 1;
			completedprocesses++;
		}
		// Increase the runtime by 1, reset interrupt boolean and decrement time delay.
		interrupt = false;
		timedelay--;
		runTime++;
	}

	// Once all processes have been completed, calculate total and average waiting time and turnaround time.
	for (int j = 0; j < numberOfProcesses; j++)
	{
		avgwaitTime += p[j].waitTime;
		avgturnaroundTime += p[j].turnaroundTime;
		if (p[j].waitTime > maxwaitTime)
		{
			maxwaitTime = p[j].waitTime;
		}
		if (p[j].turnaroundTime > maxturnaroundTime)
		{
			maxturnaroundTime = p[j].turnaroundTime;
		}
	}
	
	// Return results in an array.
	resultarray[0] = avgwaitTime / numberOfProcesses;
	resultarray[1] = maxwaitTime;
	resultarray[2] = avgturnaroundTime / numberOfProcesses;
	resultarray[3] = maxturnaroundTime;
	return resultarray;
}

int main()
{
	int i, j, runTime = 0, sum_burstTime = 0;
	char c;
	float avgwaitTime = 0, avgturnaroundTime = 0, maxwaitTime = 0, maxturnaroundTime = 0;
	bool interrupt;

	// File reading to array here
	printf("Enter in the input test case file:");
	char filename[255];
	scanf("%254s", &filename);
	struct ProcessesData processdata = readFile(filename);
	int numberOfProcesses = processdata.processno;

	// Initializing the structure variables
	for (i = 0; i < numberOfProcesses; i++)
	{
		p[i].pid = i;
		p[i].arrivalTime = processdata.arrivaltimes[i];
		p[i].burstTime = processdata.bursttimes[i];
		p[i].remainingBurstTime = processdata.bursttimes[i];
		p[i].turnaroundTime = 0;
		p[i].waitTime = 0;

		// Variable for Completion status
		// Pending = 0
		// Completed = 1
		p[i].completed = 0;

		// Variable for sum of all Burst Times
		sum_burstTime += p[i].burstTime;
	}

	// Sorting the structure by arrival times
	sortByArrival(numberOfProcesses);

	// Set lower limit to response ratio
	float hrr = -9999;

	// Response Ratio Variable
	float temp;

	// Variable to store next process selected and previous process in the event of interrupt
	int currentprocess = 0, previousprocess = 0;

	// Variables for current time and ime quantum
	int currenttime = 0;
	int quantum = 0;

	// To track number of completed processes
	int completedprocesses = 0;

	// To track total wait time of all ready processes
	int queueTotalWaitTime = 0;

	// To track number of ready processes in queue
	int queueCount = 0;

	// To track wt(running proc)/len(waiting procs) + bt(remaining)
	double currentProcScore = 0;

	float *resultarray;
	resultarray = hrrnwithpreemption(p, processdata.processno);

	printf("\nAverage waiting time:%f\n", resultarray[0]);
	printf("Maximum waiting time:%f\n", resultarray[1]);
	printf("Average Turn Around time:%f\n", resultarray[2]);
	printf("Maximum Turn Around time:%f\n", resultarray[3]);
	return 0;
}