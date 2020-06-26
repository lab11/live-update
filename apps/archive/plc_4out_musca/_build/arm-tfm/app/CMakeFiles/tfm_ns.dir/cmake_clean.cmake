file(REMOVE_RECURSE
  " //vagrant/apps/plc_4out/_build/arm-tfm/app/tfm_ns.ld.i tfm_ns.map"
  "//vagrant/apps/plc_4out/_build/arm-tfm/app/tfm_ns.axf"
  "//vagrant/apps/plc_4out/_build/arm-tfm/app/tfm_ns.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang ASM C)
  include(CMakeFiles/tfm_ns.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
