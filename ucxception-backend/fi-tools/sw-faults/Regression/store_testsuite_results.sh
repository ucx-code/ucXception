# Used to store the result of running the injector over the test suite, so that it can later be used for testing/comparison

INJECTOR_PATH="/tmp/antscript/FaultInjector.jar" # You likely have to change this
#INJECTOR_PATH="/home/frederico/Dropbox/ucxception/FaultInjector.jar"

# Create a folder to store these results
STAMP=`date +%Y-%m-%d`
RESULTS_ROOT=`pwd`/results_$STAMP
mkdir $RESULTS_ROOT;
mkdir $RESULTS_ROOT/testsuite;
mkdir $RESULTS_ROOT/testsuite/C;
mkdir $RESULTS_ROOT/testsuite/CPP;


echo "Storing results in $RESULTS_ROOT"

# TODO: Expand to CPP also (not done right now because we are focusing on C only)
cd testsuite/C;
RESULTS_ROOT=$RESULTS_ROOT/testsuite/C;

for DIR in */ ;
do 
	echo "Running and copying $DIR";
	
	test_id=$(basename $DIR)
	echo "$test_id"
	
	cd $DIR;
	mkdir original_results;
	mv * original_results;
	cp original_results/$test_id.origin.c ./$test_id.c;
	cp original_results/$test_id.origin.c ./$test_id.origin.c;
	# Run injector (output is redirected to /dev/null, although it could be useful to be shown, in case of a problem)
	java -jar $INJECTOR_PATH ./$test_id.c -c > /dev/null
	
	
	# Copy new results to our new folder
	mkdir $RESULTS_ROOT/$test_id;
	cp ./* $RESULTS_ROOT/$test_id;
	
	# Remove the mess that we did
	rm -f ./*.c;
	mv original_results/* .
	rm -rf original_results;
	cd ..
	
done