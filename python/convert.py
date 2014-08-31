import os

ppm_files = os.listdir("ppm")

for ppm_file in ppm_files:
  png_file = ppm_file[0:-4]
  os.system("pnmtopng ppm/" + ppm_file + " > png/" + png_file + ".png")


