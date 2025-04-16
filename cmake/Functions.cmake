function (update_cached name value)
  set("${name}"
      "${value}"
      CACHE INTERNAL "*** Internal ***" FORCE)
endfunction ()

function (update_cached_list name)
  set(_tmp_list "${${name}}")
  list(APPEND _tmp_list "${ARGN}")
  list(REMOVE_DUPLICATES _tmp_list)
  update_cached(${name} "${_tmp_list}")
endfunction ()
