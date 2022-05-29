#!/bin/bash
# Returns list with PIDs that have primes.py in the command string
ps -Af | grep primes.py | grep -v "grep" | cut -d ' ' -f 2
