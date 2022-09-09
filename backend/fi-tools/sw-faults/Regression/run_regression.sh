#INJECTOR_PATH="/tmp/antscript/FaultInjector.jar" # You likely have to change this
INJECTOR_PATH="/home/frederico/Dropbox/ucxception/FaultInjector.jar"

# Red and No color in echo
RED='\033[0;31m'
NC='\033[0m' # No Color
# TODO: Expand to CPP also (not done right now because we are focusing on C only)
cd c/testsuite/C;
#cd ./testsuite/C;

for DIR in */ ;
do 
	echo "Testing $DIR";
	
	test_id=$(basename $DIR)
	echo "$test_id"
	
	cd $DIR;
	mkdir original_results;
	mv * original_results;
	cp original_results/$test_id.origin.c ./$test_id.c;
	# Run injector (output is redirect to /dev/null, although it could be useful to be shown, in case of a problem)
	# -Xdebug -Xrunjdwp:transport=dt_socket,server=y,suspend=y,address=1044 Are the flags to wait until we attach JDB (for debugging)
	#time java -Xdebug -Xrunjdwp:transport=dt_socket,server=y,suspend=y,address=1044 -jar $INJECTOR_PATH ./$test_id.c -cpp > /dev/null
	time java -jar $INJECTOR_PATH ./$test_id.c -cpp -d > /dev/null
	
	# Compare results
	
	# First, compare number of generated patch file
	numfiles_orig=(./original_results/*.patch)
	numfiles_orig_count=${#numfiles_orig[@]}

	numfiles_new=(*.patch)
	numfiles_new_count=${#numfiles_new[@]}
	
	if [ "$numfiles_new_count" -eq "$numfiles_orig_count" ]; then
		echo "The amount of created patch files match!"
	else
		echo -e "${RED}There is a different number of created patch files. Original had $numfiles_orig_count, and now it has $numfiles_new_count${NC}"
		echo "Below are the files that are different:"
		
		# Move to the original results, and recompute the patch array. So that we can run the next function
		cd ./original_results;
		numfiles_orig=(*.patch)
		cd ..;
		
		# Magic to obtain the difference between 2 sets. Copied from http://stackoverflow.com/questions/2312762/compare-difference-of-two-arrays-in-bash.
		# Credit to Ilya Bystrov		
		echo ${numfiles_new[@]} ${numfiles_orig[@]} | tr ' ' '\n' | sort | uniq -u

	fi
	
	# Then compare each patch file pair
	for patch_name in ./*.patch; 
	do 
		echo "Comparing patch $patch_name with the corresponding original"
		# -q --ignore-matching-lines=*---* --ignore-matching-lines=*+++* --ignore-matching-lines=*@@*
		# Diffs both patches, while ignoring the first 2/3 (? not sure) lines. which are the header and are always different
		diff -y --suppress-common-lines  <(tail -n +3 $patch_name) <(tail -n +3 ./original_results/$patch_name)
	done
	
	echo "Press space after seeing the results, dont CTRL+C"
	read -n 1 -s # Wait for a key press to proceed
	
	# Remove the mess that we did
	rm -f ./*.c;
	mv original_results/* .
	rm -rf original_results;
	cd ..
	
	echo "Press space to continue, CTRL+C to exit"
	read -n 1 -s # Wait for a key press to proceed
done