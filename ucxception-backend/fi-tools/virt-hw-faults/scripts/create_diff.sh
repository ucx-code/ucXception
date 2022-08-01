# Generates patch file by comparing two folders (original Xen + modified Xen)
ORIG=$1
NEW=$2
diff -ruN $ORIG $NEW > file.patch
