"""
Global configuration file that defines the properties of each campaign
"""

import framework.campaigns.AI4EU
import framework.campaigns.sw_faults_example
import framework.campaigns.ExampleCampaign
import framework.campaigns.FailureCampaign

predefined_cols = ()

main_csv_path = "table.csv" # Path to the CSV that will keep the main results

plans = (
    	#(framework.campaigns.FailureCampaign.FailureCampaign),
		#(framework.campaigns.ExampleCampaign.ExampleCampaign),
		#(framework.campaigns.AI4EU.AI4EU),
		#(framework.campaigns.sw_faults_example.SW_Faults_Example),
		)
# Custom configs. Add here
