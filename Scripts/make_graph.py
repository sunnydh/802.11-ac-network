from subprocess import call
import os

def udp():
	range220 = []
	range240 = []
	range520 = []
	range540 = []
	range580 = []
	c1220 = []
	c1240 = []
	c1520 = []
	c1540 = []
	c1580 = []
	c2220 = []
	c2240 = []
	c2520 = []
	c2540 = []
	c2580 = []
	with open('../data/udp_c1_data.txt') as f:

		range220 = (f.readline()).split(' ')
		c1220 = (f.readline()).split(' ')
		range220 = [float(i) for i in range220]
		c1220 = [float(i) for i in c1220]
		range240 = (f.readline()).split(' ')
		c1240 = (f.readline()).split(' ')
		range240 = [float(i) for i in range240]
		c1240 = [float(i) for i in c1240]
		range520 = (f.readline()).split(' ')
		c1520 = (f.readline()).split(' ')
		range520 = [float(i) for i in range520]
		c1520 = [float(i) for i in c1520]
		range540 = (f.readline()).split(' ')
		c1540 = (f.readline()).split(' ')
		range540 = [float(i) for i in range540]
		c1540 = [float(i) for i in c1540]
		range580 = (f.readline()).split(' ')
		c1580 = (f.readline()).split(' ')
		range580 = [float(i) for i in range580]
		c1580 = [float(i) for i in c1580]

		with open('c1220.dat',"w") as f:
			for x,y in zip(c1220, range220):
				f.write(str(y) + "\t" + str(x) + "\n")
		with open('c1240.dat',"w") as f:
			for x,y in zip(c1240, range240):
				f.write(str(y) + "\t" + str(x) + "\n")
		with open('c1520.dat',"w") as f:
			for x,y in zip(c1520, range520):
				f.write(str(y) + "\t" + str(x) + "\n")
		with open('c1540.dat',"w") as f:
			for x,y in zip(c1540, range540):
				f.write(str(y) + "\t" + str(x) + "\n")
		with open('c1580.dat',"w") as f:
			for x,y in zip(c1580, range580):
				f.write(str(y) + "\t" + str(x) + "\n")



		call(["gnuplot", "c1graph"])
		os.remove('c1220.dat')
		os.remove('c1240.dat')
		os.remove('c1520.dat')
		os.remove('c1540.dat')
		os.remove('c1580.dat')

	with open('../data/udp_c2_data.txt') as f:

		range220 = (f.readline()).split(' ')
		c2220 = (f.readline()).split(' ')
		range220 = [float(i) for i in range220]
		c2220 = [float(i) for i in c2220]
		range240 = (f.readline()).split(' ')
		c2240 = (f.readline()).split(' ')
		range240 = [float(i) for i in range240]
		c2240 = [float(i) for i in c2240]
		range520 = (f.readline()).split(' ')
		c2520 = (f.readline()).split(' ')
		range520 = [float(i) for i in range520]
		c2520 = [float(i) for i in c2520]
		range540 = (f.readline()).split(' ')
		c2540 = (f.readline()).split(' ')
		range540 = [float(i) for i in range540]
		c2540 = [float(i) for i in c2540]
		range580 = (f.readline()).split(' ')
		c2580 = (f.readline()).split(' ')
		range580 = [float(i) for i in range580]
		c2580 = [float(i) for i in c2580]
		
		with open('c2220.dat',"w") as f:
			for x,y in zip(c2220, range220):
				f.write(str(y) + "\t" + str(x) + "\n")
		with open('c2240.dat',"w") as f:
			for x,y in zip(c2240, range240):
				f.write(str(y) + "\t" + str(x) + "\n")
		with open('c2520.dat',"w") as f:
			for x,y in zip(c2520, range520):
				f.write(str(y) + "\t" + str(x) + "\n")
		with open('c2540.dat',"w") as f:
			for x,y in zip(c2540, range540):
				f.write(str(y) + "\t" + str(x) + "\n")
		with open('c2580.dat',"w") as f:
			for x,y in zip(c2580, range580):
				f.write(str(y) + "\t" + str(x) + "\n")

		call(["gnuplot", "c2graph"])

		os.remove('c2220.dat')
		os.remove('c2240.dat')
		os.remove('c2520.dat')
		os.remove('c2540.dat')
		os.remove('c2580.dat')


	combined220 = [x+y for x,y in zip(c1220,c2220)]
	combined240 = [x+y for x,y in zip(c1240,c2240)]
	combined520 = [x+y for x,y in zip(c1520,c2520)]
	combined540 = [x+y for x,y in zip(c1540,c2540)]
	combined580 = [x+y for x,y in zip(c1580,c2580)]
	
	with open('combined220.dat',"w") as f:
		for x,y in zip(combined220, range220):
			f.write(str(y) + "\t" + str(x) + "\n")
	with open('combined240.dat',"w") as f:
		for x,y in zip(combined240, range240):
			f.write(str(y) + "\t" + str(x) + "\n")
	with open('combined520.dat',"w") as f:
		for x,y in zip(combined520, range520):
			f.write(str(y) + "\t" + str(x) + "\n")
	with open('combined540.dat',"w") as f:
		for x,y in zip(combined540, range540):
			f.write(str(y) + "\t" + str(x) + "\n")
	with open('combined580.dat',"w") as f:
		for x,y in zip(combined580, range580):
			f.write(str(y) + "\t" + str(x) + "\n")

	call(["gnuplot", "combinedgraph"])

	os.remove('combined220.dat')
	os.remove('combined240.dat')
	os.remove('combined520.dat')
	os.remove('combined540.dat')
	os.remove('combined580.dat')

def tcp():
 	call(["gnuplot", "TCPgraph"])

if __name__ == '__main__':
	udp()
	tcp()
