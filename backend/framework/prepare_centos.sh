# A script to prepare the latest versions of Pandas and etc. in a base CentOs installation
# Be careful as it might have unintended consequences if you run it on a non-base install
yum groupinstall epel-release
yum update
yum remove python-pandas
yum remove numpy
yum install python-pip python2-psutil
pip uninstall -y numpy
pip uninstall -y pandas
pip install numpy
pip install pandas
