import gmplot
import datetime
import os
import sys, getopt

loadpath = "Coordinates\Coords.txt"
savepath = "Map Files\GPS Data - {}.html".format(datetime.datetime.today().date())

def usage():
	print "-h, --help			-displays usage"
	print "-f, --filepath		-set the filepath"
try:
    opts, args = getopt.getopt(sys.argv[1:], "hf:", ["help", "filepath="])
except getopt.GetoptError as err:
	# print help information and exit:
	print str(err)  # will print something like "option -a not recognized"
	usage()
	sys.exit(2)

for opt, arg in opts:
	if opt in ("-h", "--help"):
		usage()
		sys.exit()
	if opt in ("-f", "--filepath"):
		loadpath = "Coordinates/" + arg
	
# Gets data from log file and sorts it into a 2D array:
# [[latitude, longitude], ... etc ]
data = []
with open(loadpath, "r") as f:
	lines = f.readlines()[1:]
for line in lines:
	data.append(line[20:])
data = [x.strip().replace(',',' ').split() for x in data]

# Gets list of the latitudes and longitudes
lats = []
longs = []
for i in range(len(data)-1):
    lats.append(float(data[i][0]))
    longs.append(float(data[i][1]))

# creates map object centred about the first set of coords
gmap = gmplot.GoogleMapPlotter(lats[0], longs[0], 18)

# draws a line plot onto the map (thin blue line)
gmap.plot(lats, longs, 'blue', edge_side=5)
# saves map as html file
gmap.draw(savepath)

#launches map
os.startfile(savepath)