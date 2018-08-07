import os
import math
import cv2

def GenFrames(width, height, frame_num):
	speed = 0.98
	os.system("rm *.ppm");
	os.system("rm *.avi");
	for i in range(1,frame_num):
		r = 2*math.pow(speed, i)
		dx = 0.7*r*math.cos(0.001*i*math.pi*2)
		dy = 0.7*r*math.sin(0.001*i*math.pi*2)
		x = -1.4800315820899067434+dx
		y = -0.0040555206813212809943+dy
		
		os.system("fnart " + str(width) +" " + str(height) +" "\
			+ str(x) +" " + str(y) + " " +str(r))
		os.system("mv out.ppm " + str(i) + ".ppm")
		print(str(i) + " done")

def WriteVideo(width, height, frame_num):
	print("writing to video")
	fourcc = cv2.VideoWriter_fourcc(*'MJPG')
	video = cv2.VideoWriter("seq.avi", fourcc, 30, (width,height))
	for i in range(1,frame_num):
		frame = cv2.imread(str(i) + ".ppm");
		video.write(frame)

width = 1280
height = 720
frame_num = 1200
GenFrames(width, height, frame_num)
WriteVideo(width, height, frame_num)
