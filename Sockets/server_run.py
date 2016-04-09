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


	with open("/home/susan/Desktop/BTP/Server/log.txt","a+") as f:
		f.write("Start " + str(run) + "\n")

	print "Start ", run

	f_out = open("/home/susan/Desktop/BTP/Server/tcpdumpSTDOUT", "w")
	f_err = open("/home/susan/Desktop/BTP/Server/tcpdumpSTDERR", "w")

	cmd = "sudo tcpdump -n -s0 -w /home/susan/Desktop/BTP/Server/5_80/trace"+ str(run) +".pcap -i wlan1"
	ip = "192.168.1.2"
	port = "3300"

	first = subprocess.Popen(shlex.split(cmd), stderr=f_err, stdout=f_out)
	time.sleep(2)	
	second = subprocess.Popen(['/home/susan/Desktop/BTP/802.11-ac-network/Sockets/server.o', ip, port, '5_80', str(run)])
	
	second.wait()
	os.system("sudo kill %s" % (first.pid, ))
	f_out.close()
	f_err.close()

	with open("/home/susan/Desktop/BTP/Server/log.txt","a+") as f:
		f.write("Complete " + str(run) + "\n")

	# run += 1
	# with open("run.txt","w") as f:
	# 	f.write(str(run))

	print "End ", run, "\n------------------------------------------------"

if __name__ == '__main__':

	for i in range(1,11):
		main(i)
		time.sleep(100)