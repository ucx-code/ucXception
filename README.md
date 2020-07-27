# ucXception Fault Injection Framework

This repository contains the source-code for the ucXception Fault Injection Framework and related fault injection tools.
The organization of this repo is as follows:

* fi-tools - Folder containing all the Fault Injection tools, which can be used in conjuction with the framework or as standalone applications
	* hw-faults - Fault injection tool for transient hardware faults (soft errors) which works in most Linux environments
	* hw-faults-virt - Fault injection tool for transient hardware faults which operates in a virtualized system. It is capable of injecting in any process/kernel/hypervisor that is running inside of a VM. Depends on Xen 4.11.1 as the base
	* sw-faults - Fault injection tool for software faults (aka bugs).
* framework - Folder containting the framework itself
