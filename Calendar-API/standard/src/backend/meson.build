backend_sources = files(
  'main.c',
  # Add other .c files in backend here
)

json_c = declare_dependency(
  include_directories: include_directories('../../external/include'),
  link_args: ['-L' + meson.project_source_root() + '/external', '-lexternal']
)

backend_exe = executable(
  'backend',
  backend_sources,
  include_directories: [
    include_directories('.'),
    include_directories('../lib'),
    include_directories('../../external')
  ],
  link_with: lib_bulletin,
  dependencies: [
    json_c
  ]
)