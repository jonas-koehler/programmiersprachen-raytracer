import os

#ANIMATION
width = 900
height = 900
num_frames = 150

#RAYTRACER
raytracer_path = "./build/Release/raytracer"
raytracer_flags = "--multithreading -ss 4"

#SDF
sdf_path = "sdf"
input_sdf = "sdf/simple_base_scene.sdf"
output_sdf = "sdf/merged.sdf"

#PPM
ppm_path = "ppm"
ppm_prefix = "image"
ppm_suffix = ".ppm"

#PNG
png_path = "png"
png_prefix = "image"
png_suffix = ".png"

#MP4
mp4_path = "mp4"
mp4_output = "animation.mp4"

##########################################################################################
##########################################################################################
##########################################################################################

# read in scene
f = open(input_sdf)
sdf = ""
for line in f:
  sdf += line
f.close()

files_to_convert = []

# add animation commands
for frame in range(0, num_frames):
  ppm_file = ppm_path + "/" + ppm_prefix + "{0:03d}".format(frame) + ppm_suffix
  png_file = png_path + "/" + png_prefix + "{0:03d}".format(frame) + png_suffix
  files_to_convert += [[ppm_file, png_file]]

  sdf += "transform eye rotate 1 0 1 0\n"
  sdf += "render eye " + ppm_file + " " + str(width) + " " + str(height)

  if frame != num_frames-1:
    sdf += "\n"

# write merged sdf
f = open(output_sdf, "w")
f.write(sdf)
f.close()

#render all
os.system(raytracer_path + " " + output_sdf + " " + raytracer_flags + " ")

for ppm_file, png_file in files_to_convert:
  os.system("pnmtopng " + ppm_file + " > " + png_file)

png_wildcard = png_path + "/" + png_prefix + "%03d" + png_suffix
mp4_file = mp4_path + "/" + mp4_output
os.system("ffmpeg -r 24 -i " + png_wildcard + " " + mp4_file)