"""

	Usage python server_run.py [2_20|2_40|5_20|5_40|5_80]

"""

import shlex
import subprocess
import time
import os
import signal
import sys




def main(run, band, variant):


	with open("/home/susan/Desktop/BTP/Server/log.txt","a+") as f:
		f.write("Start " + str(run) + "\n")

	print "Start ", run

	f_out = open("/home/susan/Desktop/BTP/Server/tcpdumpSTDOUT", "w")
	f_err = open("/home/susan/Desktop/BTP/Server/tcpdumpSTDERR", "w")

	cmd = "sudo tcpdump -n -s0 -w /home/susan/Desktop/BTP/Server/" + band + "/" + variant + "/trace" + str(run) +".pcap -i wlan1"
	ip = "192.168.1.2"
	port = "3300"

	first = subprocess.Popen(shlex.split(cmd), stderr=f_err, stdout=f_out)
	time.sleep(2)
	path = "/home/susan/Desktop/BTP/Server/" + band + "/" + variant + "/stats" + str(run) + ".txt"
	second = subprocess.Popen(['/home/susan/Desktop/BTP/802.11-ac-network/Sockets/server.o', ip, port, path])
	
	second.wait()
	os.system("sudo kill %s" % (first.pid, ))
	f_out.close()
	f_err.close()

	with open("/home/susan/Desktop/BTP/Server/log.txt","a+") as f:
		f.write("Complete " + str(run) + "\n")


	print "End ", run, "\n------------------------------------------------"


if __name__ == '__main__':

	if len(sys.argv) < 2:
		print "Less no. of command line arguments"
		exit(0)

	band = sys.argv[1]
	directory = "/home/susan/Desktop/BTP/Server/" + band + "/"

	if not os.path.exists(directory):
		os.makedirs(directory)

	tcp_varinat = ["cubic", "reno", "westwood", "bic", "htcp", "highspeed", "yeah"]

	for variant in tcp_varinat:
		
		directory = "/home/susan/Desktop/BTP/Server/" + band + "/" + variant + "/"

		if not os.path.exists(directory):
			os.makedirs(directory)

		cmd = "sudo sysctl net.ipv4.tcp_congestion_control=" + variant
		subprocess.call(shlex.split(cmd))

		for i in range(1,11):
			main(i, band, variant)
			time.sleep(100)