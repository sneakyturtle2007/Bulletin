
src_dir = 'src'

api_sources = files(
  'user_management.c',
  'event_management.c',
  # Add other .c files in api here
)

json_c = declare_dependency(
  include_directories: include_directories('../../external/include'),
  link_args: ['-L' + meson.project_source_root() + '/external', '-lexternal']
)

api_exe = executable(
  'api',
  api_sources,
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
