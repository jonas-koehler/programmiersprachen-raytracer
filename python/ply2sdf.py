import sys

# main
if __name__ == "__main__":

  if len(sys.argv) < 4:
        print "Not enough parameters given."
        print "Usage: python ply2sdf.py <input> <output> <materialname> [--no-normals]"
        quit()

  use_normals = True
  if len(sys.argv) == 5 and sys.argv[4] == "--no-normals":
    use_normals = False

  materialname = sys.argv[3];

  ply_file = open(sys.argv[1], 'r')

  vertices = []
  vertex_normals = []
  triangles = []
  triangle_normals = []

  vertex_count = 0
  face_count = 0

  current_faces = 0

  read_header = True
  for line in ply_file:
    line = line.replace(' \n', '').replace(' \r', '')
    line = line.replace('\n', '').replace('\r', '')
    tokens = line.split(" ")

    if read_header:
      # get vertex & face count
      if "element" in tokens[0]:
        if "vertex" in tokens[1]:
          vertex_count = int(tokens[2])
        if "face" in tokens[1]:
          face_count = int(tokens[2])

      if "end_header" in tokens[0]:
        read_header = False
    else:
      # read vertices
      if len(vertices) < vertex_count:
        tokens = map(float, tokens)
        vertices += [tokens[0:3]]
        if use_normals:
          vertex_normals += [tokens[3:7]]
      # read faces
      elif current_faces < face_count:
        tokens = map(int, tokens)
        if tokens[0] == 4:
          v = map(lambda i: vertices[tokens[i]], range(1,5))
          if use_normals:
            n = map(lambda i: vertex_normals[tokens[i]], range(1,5))
            triangles += [[[v[0], v[1], v[2]], [n[0], n[1], n[2]]]]
            triangles += [[[v[0], v[2], v[3]], [n[0], n[2], n[3]]]]
          else:
            triangles += [[[v[0], v[1], v[2]]]]
            triangles += [[[v[0], v[2], v[3]]]]

        if tokens[0] == 3:
          v = map(lambda i: vertices[tokens[i]], range(1,4))
          if use_normals:
              n = map(lambda i: vertex_normals[tokens[i]], range(1,4))
              triangles += [[v,n]]
          else:
            triangles += [[v]]

          current_faces += 1

  index = 0
  sdf = ""
  triangle_str = ""
  for triangle in triangles:
    #print triangle
    vertices = map(lambda l: " ".join(map(str, l)), triangle[0])
    if use_normals:
      normals = map(lambda l: " ".join(map(str, l)), triangle[1])
      sdf += "define shape triangle_n tn" + str(index)
      sdf += "    " + "   ".join(vertices)
      sdf += "    " + "   ".join(normals)
      sdf += " " + materialname + "\n"
      triangle_str += " tn" + str(index)
    else:
      sdf += "define shape triangle t" + str(index)
      sdf += "    " + "   ".join(vertices)
      sdf += " " + materialname + "\n"
      triangle_str += " t" + str(index)

    index += 1

  sdf += "define shape composite mesh " + triangle_str


  sdf_file = open(sys.argv[2], 'w')
  sdf_file.write(sdf)
