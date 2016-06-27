from os import listdir
from os.path import isfile
from subprocess import call
import sys
import os

project = sys.argv[1]
paths = ["mytests", "golden"] # ["mytests", "golden", "bonus"]
extensions = ["tokens", "syntax", "netlist", "evl_output"]
pathsep = "/"
errors = 0
tests = 0

if project == "BONUS":
	paths = ["bonus"]
	project = "SIM"

for mypath in paths:
	for extension in extensions:
		deletetokens = "".join(("rm -f ", mypath,"/*.",extension))
		os.system(deletetokens)
		deletetokens = "".join((deletetokens, ".golden"))
		os.system(deletetokens)

	testfiles = [ f for f in listdir(mypath) if (isfile(pathsep.join((mypath,f))) and f.endswith(".evl")) ]

	for file in testfiles:
		print(pathsep.join(("Testing ", mypath, file)))
		if (call(["./myevl", pathsep.join((mypath,file))]) == 0):
			if (call(["golden/EasyVL", pathsep.join((mypath,file)), "".join(("proj=",project)), "testing=true"]) != 0):
				errors += 1
		else:
			print("Own simulator thrown an error, aborting test")
			errors += 1
		print("")
		tests += 1

if (errors != 0):
	print "Final report: " + str(errors) + "/" + str(tests) + " tests have been failed"
else:
	print "Final report: all test have passed successfully"
