// CPU Scheduling Method: FIFO / FCFS
// Ref: https://aarushivohra.medium.com/first-come-first-serve-job-scheduling-algorithm-with-implementation-b2fba1ef5caa

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define numberOfProcesses 5

struct process{
    int processNum;
    int arrivalTime;
    int burstTime;
    int waitTime;
    int turnAroundTime;
    int completedTime;
    int completed; // 1 = Completed, 0 = Pending
}typedef process;

int processCmp(process p1, process p2){
    if(p1.arrivalTime<p2.arrivalTime)
        return 1;
    if(p1.arrivalTime==p2.arrivalTime)
        return p1.processNum<p2.processNum;   //if arrival times are equal compare pid
    return 0;
};

void findWaitingTime(process process[], int n)
{
    int service_time[n];
    service_time[0] = 0;
    process[0].waitTime = 0;
 
    for (int i = 1; i < n ; i++)
    {
        service_time[i] = service_time[i-1] + process[i-1].burstTime;
 
        process[i].waitTime = service_time[i] - process[i].arrivalTime;
 
        if (process[i].waitTime < 0)
            process[i].waitTime = 0;
    }

    for (int i = 0; i < n; i++){
        printf("Process: %d\n", process[i].processNum);
        printf("Wait Time: %d\n", process[i].waitTime);
        printf("\n");
    }
}

void findCompletionTime(process process[], int n) 
{   


    // int prevCompletedTime = process[0].burstTime;
    // process[0].completedTime = process[0].burstTime;
    // int j;
	// for (j = 1; j <= n ; j++){
    //     process[j].completedTime = process[j].burstTime + prevCompletedTime;
    //     prevCompletedTime = process[j].completedTime;
    // }


    for (int i = 0; i < n; i++){
        // printf("Process: %d\n", process[i].processNum);
        // printf("Arrival Time: %d\n", process[i].arrivalTime);
        // printf("Burst Time: %d\n", process[i].burstTime);
        // printf("Completed Time: %d\n", process[i].completedTime);
        // printf("Wait Time: %d\n", process[i].waitTime);
        // printf("\n");
        process[i].completedTime = process[i].turnAroundTime + process[i].arrivalTime;
    }
}

int biggestWaitingTimeProcess(process arr[]){
    //Initialize min with first element of array.    
    int min = arr[0].waitTime;
      
    //Loop through the array    
    for (int i = 0; i < numberOfProcesses; i++) {     
        //Compare elements of array with min    
        if(arr[i].waitTime > min){
            min = arr[i].waitTime; 
        }  
            
    }      
    return min;
}

int biggestTurnAroundTimeProcess(process arr[]){
    //Initialize min with first element of array.    
    int min = arr[0].turnAroundTime;
      
    //Loop through the array    
    for (int i = 0; i < numberOfProcesses; i++) {     
        //Compare elements of array with min    
        if(arr[i].turnAroundTime > min){
            min = arr[i].turnAroundTime; 
        }  
            
    }      
    return min;
}

void findTurnAroundTime(process process[], int n) 
{ 
	for (int i = 0; i < n ; i++) 
		process[i].turnAroundTime = process[i].burstTime + process[i].waitTime; 
}

void findavgTime(process process[], int n) 
{ 
    int total_wt = 0, total_tat = 0, total_rt = 0; 

   findWaitingTime(process, n); 
   findTurnAroundTime(process, n); 
   findCompletionTime(process, n);


   printf("Process ID \t Arrival time \t Burst time \t Wait time \t Turnaround time \t Completed Time\n"); 

   for (int i=0; i<n; i++) 
    { 
        total_wt += process[i].waitTime; 
        total_tat += process[i].turnAroundTime; 
        printf("%d \t\t %d \t\t %d \t\t %d \t\t %d \t\t\t %d\n", process[i].processNum, process[i].arrivalTime, process[i].burstTime, process[i].waitTime, process[i].turnAroundTime, process[i].completedTime);
    } 

    printf("Average waiting time: %.2f\n",(float)total_wt / (float)n);
    printf("Max waiting time: %d\n", biggestWaitingTimeProcess(process));
    printf("Average turn around time: %.2f\n",(float)total_tat / (float)n);
    printf("Max Turnaround time: %d\n", biggestTurnAroundTimeProcess(process));  
}

void sortByArrivalTime(process arr[]){
    int i, j;

    process temp;
    
    // Selection Sort
    for (i = 0; i < numberOfProcesses - 1; i++) {
        for (j = i + 1; j < numberOfProcesses; j++) {
 
            // Check for lesser arrival time
            if (arr[i].arrivalTime > arr[j].arrivalTime) {
 
                // Swap earlier process to front
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int main () {

    FILE* fp;
    char  line[10];
    int count = 1;
    int i = 0;
    int sumBurstTime = 0;
    process process_arr[numberOfProcesses];

    fp = fopen("testcase3.txt" , "r");
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        const char* arrival = strtok(line, " ");
        const char* burst = strtok(NULL, " ");
        process_arr[i].processNum = count;
        process_arr[i].arrivalTime = atoi(arrival);
        process_arr[i].burstTime = atoi(burst);
        process_arr[i].completed = 0;
        sumBurstTime += atoi(burst);
        count += 1;
        i += 1;
    }

    sortByArrivalTime(process_arr);
    findavgTime(process_arr, numberOfProcesses);

     
    return 0;
}