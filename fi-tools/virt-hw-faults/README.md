# Hardware fault injection inside of VMs

This fault injection tool allows its user to inject hardware faults (more specifically, single bit-flips in CPU registers, emulating transient hardware faults) inside of VMs.
It can be used to easily inject faults in hypervisor code that is running inside of a VM (i.e., using nested virtualization), or to simply perform injection in userspace and kernelspace applications running in a VM.
## Compilation
This tool was developed as a set of modifications to the Xen 4.11.1. These modifications add a new hypercall and respective toolstack functions to request and perform the injection. As such, to compile the tool, first, one needs to obtain the original source code of Xen 4.11.1, which should then be modified to include/overwrite the files that are inside of the 'source' folder.
To facilitate this process, the 'scripts' folder contains some Bash scripts that automatically copy the files into a Xen folder (namely 'replace_xen_sources.sh').

## Usage
After the modified Xen has been compiled and installed, it is possible to request fault injection using the command:
> _xl inject-fault < domid > < bit > < register > [range_start] [range_end]_
	
After injection takes place, a verbose output shall appear in 'xl dmesg'.
