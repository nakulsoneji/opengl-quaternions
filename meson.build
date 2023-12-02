project('gltest', 'c', 'cpp')

glfw = dependency('glfw3')
inc = include_directories('include', 'shaders')

executable('app', 'src/app.cpp', 'src/glad.c', 'src/shader.cpp', 'src/stb_image.c',
  include_directories: inc,
  dependencies: [glfw])



