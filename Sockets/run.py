import shlex
import subprocess
import time
import os
import signal




def main():

	run = 0

	if (not(os.path.isfile("./run.txt"))):
		with open("run.txt","w") as f:
			f.write("1")

	with open("run.txt","r") as f:
		run = int(f.readline())


	with open("log.txt","a+") as f:
		f.write("Start " + str(run) + "\n")

	print "Starting process 1..."

	f_out = open("tcpdumpSTDOUT", "w")
	f_err = open("tcpdumpSTDERR", "w")

	cmd = "sudo tcpdump -n -s0 -w myPackets"+ str(run) +".pcap -i eth0"
	ip = "192.168.1.1"
	port = "3300"

	first = subprocess.Popen(shlex.split(cmd), stderr=f_err, stdout = f_out)
	second = subprocess.Popen(['/home/sunman/Desktop/client.o', ip, port, str(run)])

	print "Waiting for process 2..."		
	
	second.wait()
	os.system("sudo kill %s" % (first.pid, ))
	f_out.close()
	f_err.close()

	with open("log.txt","a+") as f:
		f.write("Complete " + str(run) + "\n")

	run += 1
	with open("run.txt","w") as f:
		f.write(str(run))

if __name__ == '__main__':
	main()