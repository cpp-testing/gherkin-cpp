#!/usr/bin/python3
import subprocess
import os

# prevent stack-trace on ctrl+c
import signal
import sys
signal.signal(signal.SIGINT, lambda x, y: sys.exit(0))

import filecmp

class bcolors:
  OKGREEN = '\033[32m'
  FAIL = '\033[91m'
  ENDC = '\033[0m'

targets = []

# collect the list of all tests
for root, dirs, files in os.walk("."):
    for file in files:
        if file.endswith(".feature"):
            targets.append(os.path.join(root, file))

overal_result = True

for target in targets:
    #print(target)
    target_result = target+".result"
    outfile = open(target_result, "w")
    run_result = subprocess.run(["valgrind", "--leak-check=full", "--error-exitcode=-1", "./test", target], stdout=outfile, stderr=subprocess.DEVNULL)
    #print(run_result.returncode)


    # quick hack to keep the .out files untouched but still run from the test folder
    fin = open(target_result, "rt")
    data = fin.read()
    data = data.replace('"uri":"./testdata/', '"uri":"test/testdata/')
    fin.close()
    fin = open(target_result, "wt")
    fin.write(data)
    fin.close()

    target_out = target + ".out"
    comp = filecmp.cmp(target_out, target_result)
    #print("Test run result: " + str(comp))

    fixed_target_string = "{0:>70}".format(target)
    print(fixed_target_string + " -> ", end='', flush=True)

    if comp == True:
        colored_status_code = bcolors.OKGREEN + 'OK' + bcolors.ENDC
    else:
        colored_status_code = bcolors.FAIL + 'FAIL' + bcolors.ENDC
        overal_result = False
    print(colored_status_code)

if overal_result == True:
    os._exit(0)
else:
    print("Compare the *.feature.out to the *.feature.result for each failed test")
    print("----------------------------------------------------------------------")
    print("At least one test failed, so this script exits with an error (exit code 1)")
    os._exit(1)
