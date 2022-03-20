import random

def random_arrival_times():
    arrival_list = random.sample(range(5,50),2)
    arrival_list += random.sample(range(0,10),2)
    arrival_list += random.sample(range(10,200),3)
    arrival_list.append(0)
    arrival_list.sort()
    return arrival_list

def random_burst_times():
    burst_list = random.sample(range(10,200),4)
    burst_list += random.sample(range(1,10),2)
    burst_list += random.sample(range(1,50),2)
    random.shuffle(burst_list)
    return burst_list

def generate_test_cases(n):
    for i in range(1,n+1):
        arrival_time_list = random_arrival_times()
        burst_time_list = random_burst_times()
        filename = "./testcases/testcase" + str(i) + ".txt"
        print(f"filename == {filename}, arrival_times == {arrival_time_list}, bust_times == {burst_time_list}")
        f = open(filename, "w")
        for i in range(0,8):
            f.write(str(arrival_time_list[i]) + " " + str(burst_time_list[i]))
            if (i < 7):
                f.write("\n")
        f.close()

if __name__ == '__main__':
    random.seed(10)
    no = input("How many test cases to create? ==> ")
    generate_test_cases(int(no))