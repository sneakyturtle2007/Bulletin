
database_sources = files(
  'database.c',
  'main.c'
  # Add other .c files in database here
)

json_c = declare_dependency(
  include_directories: include_directories('../../external/include'),
  link_args: ['-L' + meson.project_source_root() + '/external', '-lexternal']
)

database_exe = executable(
  'database',
  database_sources,
  include_directories: [
    include_directories('./include'),
    include_directories('../lib'),
    include_directories('../../external')
  ],
  link_with: lib_bulletin,
  dependencies: [
    json_c
  ]
)