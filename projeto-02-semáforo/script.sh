#!/bin/bash
 
export myrun_DIR=`pwd`
export myrun_TEST=/tmp/test
 
rm -rf $myrun_TEST
mkdir $myrun_TEST
cd $myrun_TEST
set -x
ls $myrun_TEST
$myrun_DIR/multi_prod_cons
cd $myrun_DIR
ls $myrun_TEST
