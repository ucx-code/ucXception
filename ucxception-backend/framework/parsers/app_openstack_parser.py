class App_Openstack_Parser:
    def parse(self, current_folder, app_output):
        file = open(current_folder+"/workload.txt", "w+")
        for line in app_output:
            file.write(line)
        file.close()

        expected_output = {"X1": 6, "X2": 1, "X3": 16, "X4": 7, "X5": 34,
                           "X6": 5, "X7": 2, "X8": 0,  "X9": 0, "X10": 1,  "X11": 6}
        row = {}

        task = ""
        status = 0
        counter_output = 0
        counter_time = 0
        start_time = 0
        end_time = 0
        total_errors = 0

        for line in app_output[1:]:
            line = line.rstrip("\n")

            if line.startswith("X"):
                task = line.split(" ")[0]
            elif counter_time == 0 and line.isdecimal():
                start_time = int(line)
                counter_time = 1
            elif line.startswith("Status = "):
                status = line.replace("Status = ", "")
            elif counter_time == 1 and line.isdecimal():
                end_time = int(line)
                counter_time = 0
                row["task_"+task+"_status_code"] = str(status)
                row["task_"+task+"_total_time"] = str(end_time - start_time)

                #print("Tak = ", task)
                #print("Counter output: ", counter_output)
                #print("Expected output: ", expected_output[task])

                if counter_output == expected_output[task]:
                    row["task_"+task+"_expected_output"] = "correct output " + \
                        str(counter_output)
                elif counter_output > expected_output[task]:
                    row["task_"+task +
                        "_expected_output"] = "Output bigger than expected " + str(counter_output)
                    total_errors = total_errors + 1
                elif counter_output < expected_output[task]:
                    row["task_"+task +
                        "_expected_output"] = "Output less than expected = " + str(counter_output)
                    total_errors = total_errors + 1

                counter_output = 0
                task = ""
                status = 0
            else:
                counter_output = counter_output + 1

        row["total_errors"] = total_errors
        return row