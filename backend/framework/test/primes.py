""" Example program in python to calculate all prime values until a certain limit
"""
import sys

# Copied from https://hackernoon.com/prime-numbers-using-python-824ff4b3ea19
def calcPrime(givenNumber):
    # Initialize a list
    primes = []
    for possiblePrime in range(2, givenNumber + 1):
        # Assume number is prime until shown it is not.
        isPrime = True
        for num in range(2, int(possiblePrime ** 0.5) + 1):
            if possiblePrime % num == 0:
                isPrime = False
                break
        if isPrime:
            primes.append(possiblePrime)

    return (primes)


if __name__ == "__main__":
    limit = int(sys.argv[1])
    print(calcPrime(limit))