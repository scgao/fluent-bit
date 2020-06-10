set(src
  gce_metadata.c
  stackdriver_conf.c
  stackdriver.c
  stackdriver_special_fields.c
  )

FLB_PLUGIN(out_stackdriver "${src}" "")
