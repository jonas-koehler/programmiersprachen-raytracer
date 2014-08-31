width = 300
height = 300
mesh_file = "sdf/dragon.sdf"
out_file = "sdf/final.sdf"

sdf = ""

#materials
sdf += "define material    red    0.1 0.1 0.1   0.5 0 0   0.5 0.5 0.5  200   1   1 \n"
sdf += "define material    blue    0.1 0.1 0.1   0 0 0.5   0.5 0.5 0.5  200   1   1 \n"
sdf += "define material    test    0.1 0.1 0.1   1.0 1.0 1.0   0.1 0.1 0.1  200   1   1 \n"
sdf += "define material    grey    0.1 0.1 0.1   0.8 0.8 0.8   0.2 0.2 0.2  200   0.1   1.6 \n"

#camera
sdf += "define camera eye 60.0  0 0 0  0 0 1  0 1 0  \n"
sdf += "transform eye translate 0 3 15 \n"


# lights
sdf += "define light spot0   100 100 100   0.3 0.2 0.1   1.0 0.5 0.1 \n"
sdf += "define light spot1   -100 100 100   0.1 0.2 0.3   0.5 0.6 0.8 \n"

# floor

sdf += "define shape box floor -1 -1 -1 1 1 1 test\n"
sdf += "transform floor scale 100 1 100\n"
sdf += "transform floor translate 0 -2 0 \n"

# mesh
f = open(mesh_file, 'r')
for line in f:
  sdf += line
sdf += "\n"

sdf += "transform mesh rotate 45 0 1 0\n"
sdf += "transform mesh scale 0.5 0.5 0.5 \n"
sdf += "transform mesh translate 0.0 0.5 0.0 \n"

#animation frames
i=0
for i in range(0, 0):
  sdf += "transform eye rotate 2.6 0 1 0 \n"
  #sdf += "transform mesh rotate 10 1 0 0 \n"
  sdf += "render eye ppm/image" + "{0:03d}".format(i) + ".ppm " + str(width) + " " + str(height) + " \n"

sdf += "render eye ppm/image" + "{0:03d}".format(i + 1) + ".ppm " + str(width) + " " + str(height)


f = open(out_file, 'w')
f.write(sdf)
