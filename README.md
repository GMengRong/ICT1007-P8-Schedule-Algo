# ICT1007-P8-Schedule-Algo

## How to use HRRN+1 with preemption scheduling algorithm:
1. Clone the main branch into your local machine <br>
```git clone https://github.com/Xynorimas/ICT1007-P8-Schedule-Algo```
2. Create testcases in text file and put in the "code" folder <br>
![image](https://user-images.githubusercontent.com/23615745/161418712-4948c710-1421-415a-8f69-5d0cb753c029.png)
3. Compile and run "assignment.c" and select the testcase to be tested<br>
![image](https://user-images.githubusercontent.com/23615745/161418844-c69e4351-73dd-4544-a230-de439fcc4850.png)

## How to use the algorithm tester to compare the schedulers:
1. Clone the main branch into your local machine <br>
```git clone https://github.com/Xynorimas/ICT1007-P8-Schedule-Algo``` 
2. Run "testCaseGenerator.py" in the "Algorithm_Tester" folder to generate testcases to be tested <br>
![image](https://user-images.githubusercontent.com/23615745/161418913-a35eda2e-84ff-4f13-8abb-365f9f347102.png)
3. A testcases folder should be created and sample testcases should be populated in the folder. <br>
![image](https://user-images.githubusercontent.com/23615745/161418961-ab3dd744-bad4-42e0-b7c9-2589091c47e4.png)
4. In the event that you would like to run the algorithm tester against your own testcases, please put the testcases in the "testcases" folder following the naming convention of the testcase generator. eg. ("testcase1.txt" - "testcase100.txt").
5. Compile and run "algorithm_tester.c" and select the number of testcases to be run depending on the number of testcases generated. <br>
![image](https://user-images.githubusercontent.com/23615745/161419018-94f1b82f-a91d-4561-889b-3a91dcf1abf6.png)<br>
*Following the example above, we created 100 test cases and will now run the algorithm tester against 100 testcases*
6. The results of the algorithm tester will be saved to a file called "hrrnresults.csv" for your perusal.<br>
![image](https://user-images.githubusercontent.com/23615745/161419082-e6b0095a-fe42-4a78-b060-35f81086a4af.png)
