import csv

def is_float_try(str):
    try:
        value = float(str)
        return value
    except ValueError:
        return str
    
def statistics_parser(filepath):

    dicio_statistics = {}
    data_chart = []
    
    try:
        with open(filepath, mode='r') as csv_file:
            csv_reader = csv.DictReader(csv_file)
            
            golden_lines = 0
            fault_lines = 0
            md5_parser_use = False
            
            for row in csv_reader:
                #Bit in row = Fault run
                if "fi_enabled" in row and row["fi_enabled"] == "True":
                    #First iteration, dicio is empty, set all values
                    if "fault_run_mean_peak_duration" not in dicio_statistics:
                        
                        if "duration" in row:
                            dicio_statistics["fault_run_mean_peak_duration"] = dicio_statistics["fault_run_total_duration"] = float(row["duration"])
                            dicio_statistics["fault_run_min_peak_duration"] = dicio_statistics["fault_run_max_peak_duration"] = round(float(row["duration"]),2)
                        
                        dicio_statistics["percentage_crashes"] = dicio_statistics["percentage_watchdogs_fault_crashes"] = 0

                        if "output_matches" in row:
                            dicio_statistics["percentage_incorrect_output"] = 0
                            md5_parser_use = True

                    else:
                        #Sum next duration
                        dicio_statistics["fault_run_mean_peak_duration"] += float(row["duration"])
                        dicio_statistics["fault_run_total_duration"] += float(row["duration"])
                        
                        #Update min and max values
                        if round(float(row["duration"]),2) > dicio_statistics["fault_run_max_peak_duration"]:
                            dicio_statistics["fault_run_max_peak_duration"] = round(float(row["duration"]),2)
                        if round(float(row["duration"]),2) < dicio_statistics["fault_run_min_peak_duration"]:
                            dicio_statistics["fault_run_min_peak_duration"] = round(float(row["duration"]),2)
                    
                    if "app_rcode" in row:
                        if float(row["app_rcode"]) != 0.0:
                            dicio_statistics["percentage_crashes"] += 1
                    
                        if float(row["app_rcode"]) == -9.0:
                            dicio_statistics["percentage_watchdogs_fault_crashes"] += 1
                        

                    if "output_matches" in row and row["output_matches"] == "False":
                        dicio_statistics["percentage_incorrect_output"] += 1

                    #Count fault runs
                    fault_lines = fault_lines + 1

                    data_chart.append({"run_n": fault_lines, "all_type_crashes": round(((dicio_statistics["percentage_crashes"]/fault_lines) * 100),2), "watchdog_type_crashes": round(((dicio_statistics["percentage_watchdogs_fault_crashes"]/fault_lines) * 100),2)})
                #Bit not in row = Golden run
                else:
                    #First iteration, dicio is empty, set all values
                    if "golden_run_mean_peak_duration" not in dicio_statistics:

                        if "duration" in row:
                            dicio_statistics["golden_run_mean_peak_duration"] = dicio_statistics["golden_run_total_duration"] = float(row["duration"])
                            dicio_statistics["golden_run_min_peak_duration"] = dicio_statistics["golden_run_max_peak_duration"] = round(float(row["duration"]),2)
                    
                    else:
                        #Add next duration
                        dicio_statistics["golden_run_mean_peak_duration"] += float(row["duration"])
                        dicio_statistics["golden_run_total_duration"] += float(row["duration"])
                        
                        #Update min and max values
                        if round(float(row["duration"]),2) > dicio_statistics["golden_run_max_peak_duration"]:
                            dicio_statistics["golden_run_max_peak_duration"] =  round(float(row["duration"]),2)
                        if round(float(row["duration"]),2) < dicio_statistics["golden_run_min_peak_duration"]:
                            dicio_statistics["golden_run_min_peak_duration"] =  round(float(row["duration"]),2)

                    #Count golden runs
                    golden_lines =  golden_lines + 1


            #Mean calculation
            if "fault_run_mean_peak_duration" in dicio_statistics:
                dicio_statistics["fault_run_mean_peak_duration"] = round( (dicio_statistics["fault_run_mean_peak_duration"]/fault_lines) , 2)
            if "golden_run_mean_peak_duration" in dicio_statistics:
                dicio_statistics["golden_run_mean_peak_duration"] = round( (dicio_statistics["golden_run_mean_peak_duration"]/golden_lines) , 2)

            if  "fault_run_total_duration" in dicio_statistics:
                dicio_statistics["fault_run_total_duration"] = round( dicio_statistics["fault_run_total_duration"] , 2)
            if  "golden_run_total_duration" in dicio_statistics:
                dicio_statistics["golden_run_total_duration"] = round( dicio_statistics["golden_run_total_duration"] , 2)

            #Percentage of crashes
            if "percentage_crashes" in dicio_statistics:
                dicio_statistics["percentage_crashes"] = round(((dicio_statistics["percentage_crashes"]/fault_lines) * 100),2)
            if "percentage_watchdogs_fault_crashes" in dicio_statistics:
                dicio_statistics["percentage_watchdogs_fault_crashes"] = round(((dicio_statistics["percentage_watchdogs_fault_crashes"]/fault_lines) * 100),2)

            #MD5 Parser
            if md5_parser_use:
                if "percentage_incorrect_output" in dicio_statistics:
                    dicio_statistics["percentage_incorrect_output"] = round(( (dicio_statistics["percentage_incorrect_output"]/fault_lines) * 100) - dicio_statistics["percentage_crashes"],2)
            
            #Data for chart
            if data_chart:
                dicio_statistics["data_chart_crashes"] = data_chart

    except IOError:
        print("Could not read file:" + filepath)
    
    return dicio_statistics

def columns_parser(filepath):

    try:
        with open(filepath, mode='r') as csv_file:
            csv_reader = csv.reader(csv_file, delimiter = ',')
        
            # loop to iterate through the rows of csv
            for row in csv_reader:
                
                #Return first row
                return row

    except IOError:
        print("Could not read file:" + filepath)
    
    return []
    
def chart_parser(filepath, xaxis, yaxis):
    
    chartdata = []
    
    try:
        with open(filepath, mode='r') as csv_file:
            csv_reader = csv.DictReader(csv_file)

            for row in csv_reader:

                if xaxis not in row or yaxis not in row:
                    break
                
                if "fi_enabled" in row and row["fi_enabled"] == "True":
                    xaxis_value = is_float_try(row[xaxis])
                    yaxis_value = is_float_try(row[yaxis])

                    chartdata.append({"xaxis_value": xaxis_value, "yaxis_value": yaxis_value})
                
    except IOError:
        print("Could not read file:" + filepath)
    
    return chartdata