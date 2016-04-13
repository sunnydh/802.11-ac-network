
def compute_average():

	lines = [None]*10
	f = [None]*10
	with open('/home/susan/Desktop/BTP/Client/2.4_20/stats1.txt', 'r') as f[0], open('/home/susan/Desktop/BTP/Client/2.4_20/stats2.txt', 'r') as f[1], open('/home/susan/Desktop/BTP/Client/2.4_20/stats3.txt', 'r') as f[2], open('/home/susan/Desktop/BTP/Client/2.4_20/stats4.txt', 'r') as f[3], open('/home/susan/Desktop/BTP/Client/2.4_20/stats5.txt', 'r') as f[4], open('/home/susan/Desktop/BTP/Client/2.4_20/stats6.txt', 'r') as f[5], open('/home/susan/Desktop/BTP/Client/2.4_20/stats7.txt', 'r') as f[6], open('/home/susan/Desktop/BTP/Client/2.4_20/stats8.txt', 'r') as f[7], open('/home/susan/Desktop/BTP/Client/2.4_20/stats9.txt', 'r') as f[8], open('/home/susan/Desktop/BTP/Client/2.4_20/stats10.txt', 'r') as f[9]:
		for i in range(0, 10):
			lines[i] = f[i].readlines()

		with open('/home/susan/Desktop/BTP/Client/2.4_20/stats_avg.txt', 'w+') as f_out:
				for j, line in enumerate(lines[0]):
					avg_cwnd = avg_time = avg_rtt = avg_ssthresh = 0.0
					for i in range(0, 10):
						avg_cwnd += float(lines[i][j].split("\t")[1])
						avg_time += float(lines[i][j].split("\t")[0])
						avg_ssthresh += float(lines[i][j].split('\t')[2])
						avg_rtt += float(lines[i][j].split("\t")[3])

					avg_cwnd = avg_cwnd/10
					avg_time = avg_time/10
					avg_ssthresh = avg_ssthresh/10
					avg_rtt = avg_rtt/10 
					write_str = str(avg_time) + "\t" + str(avg_cwnd) + "\t" + str(avg_ssthresh) + "\t" + str(avg_rtt) + "\n"
					f_out.write(write_str)

if __name__ == '__main__':
	compute_average()
