# ucXception Fault Injection Framework

This repository contains the source-code for the ucXception Fault Injection Framework and related fault injection tools.
The organization of this repo is as follows:

* backend - Contains the core of ucXception
	* fi-tools - Contains all the Fault Injection tools, which can be used in conjuction with the framework or as standalone applications
		* hw-faults - Fault injection tool for transient hardware faults (soft errors) which works in most Linux environments
		* hw-faults-virt - Fault injection tool for transient hardware faults which operates in a virtualized system. It is capable of injecting in any process/kernel/hypervisor that is running inside of a VM. Depends on Xen 4.11.1 as the base
		* sw-faults - Fault injection tool for software faults (aka bugs).
		* failure - Fault (failure) injection tool that emulates a crash failure of a Linux process (calls kill).
	* framework - Contains the framework (the manager) itself
	* api - Contains the REST API that the frontend uses
	* csv-files - Empty folder that is used to store the result CSVs
	* private-key - Holds the private key (used in the Docker container for accessing and conducting fault injection in remote hosts)
* frontend - Contains the Web UI of ucXception
	* public - Unimportant files for the webpage
	* src - Source code of the web UI (React)
* docs - Contains various docs that were used during the development

# Docker images

There are pre-made Docker images available at https://hub.docker.com/r/ucxcode/ucxception/tags and there is an example docker-compose.yml that you can adapt and use. You can also choose to manually build the Docker images using the Dockerfiles available in the frontend and backend folders.
