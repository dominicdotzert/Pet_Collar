import gmplot
import datetime
import os
import sys, getopt

loadpath = "Coordinates/Coords.txt"
savepath = "Map Files/GPS Data - {}.html".format(datetime.datetime.today().date())

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
	
data = []
with open(loadpath, "r") as f:
    for line in f:
        data.append(line)
data = [x.strip().replace(',','').split() for x in data]

lats = []
longs = []
for i in range(len(data)):
    lats.append(float(data[i][0]))
    longs.append(float(data[i][1]))

gmap = gmplot.GoogleMapPlotter(lats[0], longs[0], 18)

gmap.plot(lats, longs, 'blue', edge_side=5)
gmap.draw(savepath)

os.startfile(savepath)