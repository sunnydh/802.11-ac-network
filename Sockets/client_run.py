import shlex
import subprocess
import time
import os
import signal




def main(run):

	# run = 0

	# if (not(os.path.isfile("./run.txt"))):
	# 	with open("run.txt","w") as f:
	# 		f.write("1")

	# with open("run.txt","r") as f:
	# 	run = int(f.readline())


	with open("log.txt","a+") as f:
		f.write("Start " + str(run) + "\n")

	print "Start ", run

	f_out = open("tcpdumpSTDOUT", "w")
	f_err = open("tcpdumpSTDERR", "w")

	cmd = "sudo tcpdump -n -s0 -w /home/susan/Desktop/BTP/Client/2.4_20/trace"+ str(run) +".pcap -i wlan2"
	ip = "192.168.1.2"
	port = "3300"

	first = subprocess.Popen(shlex.split(cmd), stderr=f_err, stdout = f_out)
	time.sleep(2)	
	second = subprocess.Popen(['/home/susan/Desktop/BTP/802.11-ac-network/Sockets/client.o', ip, port, str(run)])
	
	second.wait()
	os.system("sudo kill %s" % (first.pid, ))
	f_out.close()
	f_err.close()

	with open("log.txt","a+") as f:
		f.write("Complete " + str(run) + "\n")

	# run += 1
	with open("run.txt","w") as f:
		f.write(str(run))

	print "End ", run

if __name__ == '__main__':

	for i range(1,21):
		main(i)
		time.sleep(60)