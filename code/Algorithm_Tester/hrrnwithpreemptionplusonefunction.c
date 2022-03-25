float* hrrnwithpreemptionplusone (struct process p[], int numberOfProcesses) {
	static float resultarray[4];
	// Set lower limit to response ratio
	float hrr = -9999;

	// Response Ratio Variable
	float temp;
	
	bool interrupt;
	
	float avgwaitTime = 0, avgturnaroundTime = 0, maxwaitTime = 0, maxturnaroundTime = 0;

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

			if (queueTotalWaitTime > p[currentprocess].remainingBurstTime && p[currentprocess].completed != 1)
			{
				previousprocess = currentprocess;
				interrupt = true;
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
						// temp = (float)(p[i].burstTime + (runTime - p[i].arrivalTime)) / (float)p[i].burstTime;
						temp = (float)(p[i].remainingBurstTime + (runTime - p[i].arrivalTime) +1) / (float)p[i].remainingBurstTime;

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
							// if (p[currentprocess].burstTime > p[i].burstTime)
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
		// Increase the runtime by 1
		interrupt = false;
		runTime++;
	}

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
	
	resultarray[0] = avgwaitTime / numberOfProcesses;
	resultarray[1] = maxwaitTime;
	resultarray[2] = avgturnaroundTime / numberOfProcesses;
	resultarray[3] = maxturnaroundTime;
	return resultarray;
}
