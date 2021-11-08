"""
Global configuration file that defines the properties of each campaign
"""

import campaigns.AI4EU
import campaigns.sw_faults_example

predefined_cols = ()

main_csv_path = "table.csv" # Path to the CSV that will keep the main results

plans = (
			(campaigns.AI4EU.AI4EU),
			#(campaigns.sw_faults_example.SW_Faults_Example),
		)
# Custom configs. Add here
