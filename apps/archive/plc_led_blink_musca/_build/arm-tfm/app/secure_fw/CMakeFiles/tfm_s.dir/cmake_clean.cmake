file(REMOVE_RECURSE
  " //vagrant/apps/plc_led_blink/_build/arm-tfm/app/s_veneers.o //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/tfm_s.ld.i tfm_s.map"
  "//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/tfm_s.axf"
  "//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/tfm_s.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang ASM C)
  include(CMakeFiles/tfm_s.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
