"""
Global configuration file that defines the properties of each campaign
"""
import campaigns.local_exec
import campaigns.AI4EU

predefined_cols = ()

main_csv_path = "table.csv" # Path to the CSV that will keep the main results

plans = (
			#(campaigns.local_exec.Local_Exec),
			(campaigns.AI4EU.AI4EU),
		)
# Custom configs. Add here