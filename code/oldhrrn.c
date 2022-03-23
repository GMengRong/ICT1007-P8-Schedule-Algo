// C program for Highest Response Ratio Next (HRRN) Scheduling
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defining process details
struct process {
	char name;
	int at, bt, ct, wt, tt;
	int completed;
	float ntt;
}p[255];

int n;

// Sorting Processes by Arrival Time
void sortByArrival()
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

struct ProcessesData
{
    int arrivaltimes[255];
    int bursttimes[255];
    int processno;
};

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

void main()
{
	int i, j, t, sum_bt = 0;
	char c;
	float avgwt = 0, avgtt = 0;

	//File reading to array here
	printf("Enter in the input test case file:");
    char filename[30];
    scanf("%28s", &filename);
    struct ProcessesData processdata = readFile(filename);
	n = processdata.processno;

	// Initializing the structure variables
	for (i = 0, c = 'A'; i < n; i++, c++) {
		p[i].name = c;
		p[i].at = processdata.arrivaltimes[i];
		p[i].bt = processdata.bursttimes[i];

		// Variable for Completion status
		// Pending = 0
		// Completed = 1
		p[i].completed = 0;

		// Variable for sum of all Burst Times
		sum_bt += p[i].bt;
	}

	// Sorting the structure by arrival times
	sortByArrival();

	printf("\nName\tArrival Time\tBurst Time\tWaiting Time");
	printf("\tTurnAround Time\t Normalized TT");
	for (t = p[0].at; t < sum_bt;) {

		// Set lower limit to response ratio
		float hrr = -9999;

		// Response Ratio Variable
		float temp;

		// Variable to store next process selected
		int loc;
		for (i = 0; i < n; i++) {

			// Checking if process has arrived and is Incomplete
			if (p[i].at <= t && p[i].completed != 1) {

				// Calculating Response Ratio
				temp = (float)(p[i].bt + (t - p[i].at)) / (float)p[i].bt;
				//printf("\n%c temp = %d\n", p[i].name, temp);
				//printf("\n\t%c:%d + (%d - %d)/%d = %f\n", p[i].name, p[i].bt, t, p[i].at, p[i].bt, temp);

				// Checking for Highest Response Ratio
				if (hrr < temp) {

					// Storing Response Ratio
					hrr = temp;
					// printf("%f", hrr);

					// Storing Location
					loc = i;
				}
				else if(hrr == temp){
					if (p[loc].bt > p[i].bt){
						loc = i;
					}
				}
			}
		}

		// Updating time value
		t += p[loc].bt;

		// Calculation of waiting time
		p[loc].wt = t - p[loc].at - p[loc].bt;

		// Calculation of Turn Around Time
		p[loc].tt = t - p[loc].at;

		// Sum Turn Around Time for average
		avgtt += p[loc].tt;

		// Calculation of Normalized Turn Around Time
		p[loc].ntt = ((float)p[loc].tt / p[loc].bt);

		// Updating Completion Status
		p[loc].completed = 1;

		// Sum Waiting Time for average
		avgwt += p[loc].wt;
		printf("\n%c\t\t%d\t\t", p[loc].name, p[loc].at);
		printf("%d\t\t%d\t\t", p[loc].bt, p[loc].wt);
		printf("%d\t\t%f", p[loc].tt, p[loc].ntt);

	}

    float maxwaitTime = 0.0, maxturnaroundTime = 0.0;
    
    for (j = 0; j < n; j++)
	{
		if (p[j].wt > maxwaitTime){
			maxwaitTime = p[j].wt;
		}
		if (p[j].tt > maxturnaroundTime) {
			maxturnaroundTime = p[j].tt;
		}
	}
    
	printf("\nAverage waiting time:%f\n", avgwt / n);
    printf("Maximum waiting time:%f\n", maxwaitTime);

	printf("Average Turn Around time:%f\n", avgtt / n);
	printf("Maximum Turn Around time:%f\n", maxturnaroundTime);
}