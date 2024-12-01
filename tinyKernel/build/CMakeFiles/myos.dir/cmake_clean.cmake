file(REMOVE_RECURSE
  "CMakeFiles/myos"
  "boot.o"
  "gdt.o"
  "kernel.o"
  "myos.bin"
  "string.o"
  "testfont.o"
  "vesadrv.o"
  "video.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/myos.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
