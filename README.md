# ICT1007-P8-Schedule-Algo

## How to use HRRN+1 with preemption scheduling algorithm:
1. Clone the main branch into your local machine
2. Create testcases in text file and put in the same folder
3. Compile and run "hrrnwithpreemptionWeighted.c" and select the testcase to be tested

## How to use the algorithm tester to compare the schedulers:
1. Clone the main branch into your local machine
2. Run "testCaseGenerator.py" to generate testcases to be tested
3. A testcases folder should be created and sample testcases should be populated in the folder.
4. In the event that you would like to run the algorithm tester against your own testcases, please put the testcases in the "testcases" folder following the naming convention of the testcase generator. eg. ("testcase1.txt" - "testcase100.txt").
5. Compile and run "algorithm_tester.c" and select the number of testcases to be run depending on the number of testcases generated.
6. The results of the algorithm tester will be saved to a file called "hrrnresults.csv" for your perusal.
