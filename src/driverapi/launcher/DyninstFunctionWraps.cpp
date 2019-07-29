#include "DyninstFunctionWraps.h"

std::map<std::string, DFW_wrapper> DFW_MAP;
volatile bool DFW_MAP_SET = false;
void OneTimeFillMap() {
	if (DFW_MAP_SET)
		return;
	DFW_MAP_SET = true;
	DFW_wrapper tmp;
	DFW_MAP.clear();

	// String search for char functions
	tmp = {
		.wrap =  std::string("strchr"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strchr_wrapper"),
		.argMap = {std::make_pair(0,0)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__strchr_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strchr_wrapper"),
		.argMap = {std::make_pair(0,0)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__GI_strchr"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strchr_wrapper"),
		.argMap = {std::make_pair(0,0)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	// string copy style wrapper (dest, source)
	tmp = {
		.wrap =  std::string("__strcpy_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__GI_strstr"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("__strcpy_power8"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__GI_strcpy"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	tmp = {
		.wrap =  std::string("strcpy"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__strcpy_small"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	/*
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__GI_stpcpy"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__stpcpy_power8"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__stpcpy_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("__stpcpy_ppc"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
/
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__strcmp_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__strcmp_power8"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__strcmp_ppc"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("strcmp"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	*/
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__strcasecmp_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("__strcasecmp_ppc"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	// String Cat style (dest, source) where len = len(dest) + len(source)
	tmp = {
		.wrap =  std::string("__strcat_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcat_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("strcat"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strcat_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	// string n copy stirngs (dest source len)
	tmp = {
		.wrap =  std::string("bcopy"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("strncmp"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__strncmp_power8"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__strncmp_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__strncmp_power4"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__strncmp_ppc"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("stpncpy"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__stpncpy_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__stpncpy_power8"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__GI___stpncpy"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("__strncpy_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__strncpy_power8"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("strncpy"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__strncpy_ppc"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__strncpy_chk"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("__strncasecmp_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_strncpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	
	// The memcpy like functions that share (dest, source, size) parameters
	tmp = {
		.wrap =  std::string("__memcmp_ppc"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__memcmp_power4"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__memcmp_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("memcmp"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__GI_memmove"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("__GI_memmove"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__memmove_chk"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("__memcpy_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__GI_memcpy"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("__memcpy_a2"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("__memcpy_power6"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("__memcpy_cell"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__memcpy_power4"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;	
	tmp = {
		.wrap =  std::string("__memcpy_ppc"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;	
	tmp = {
		.wrap =  std::string("__memcpy_chk"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__memcpy_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("_wordcopy_fwd_aligned"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("_wordcopy_fwd_aligned_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("_wordcopy_fwd_aligned_power6"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("_wordcopy_fwd_aligned_ppc"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("_wordcopy_fwd_dest_aligned"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("_wordcopy_fwd_dest_aligned_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("_wordcopy_fwd_dest_aligned_power6"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("_wordcopy_fwd_dest_aligned_ppc"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("_wordcopy_bwd_aligned"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;	
	tmp = {
		.wrap =  std::string("_wordcopy_bwd_dest_aligned"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("_wordcopy_bwd_aligned_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("_wordcopy_bwd_dest_aligned_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("_wordcopy_bwd_aligned_power6"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("_wordcopy_bwd_dest_aligned_power6"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("_wordcopy_bwd_aligned_ppc"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("_wordcopy_bwd_dest_aligned_ppc"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("__mempcpy_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("__mempcpy_chk"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	tmp = {
		.wrap =  std::string("mempcpy"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__GI_mempcpy"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memcpy_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	// Memset style with (dest, value, size)
	tmp = {
		.wrap =  std::string("__GI_wmemset"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memset_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__memset_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memset_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__memset_power6"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memset_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__memset_power4"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memset_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__memset_ppc"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memset_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("memset"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memset_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__libc_memset"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memset_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__memset_chk"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memset_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__memchr_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memset_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__memrchr"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memset_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;
	tmp = {
		.wrap =  std::string("__memrchr_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_memset_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(2,2)},
	};
	DFW_MAP[tmp.wrap] = tmp;

	// bzero (dest, size)
	tmp = {
		.wrap =  std::string("__bzero_power7"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_bzero_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;	
	tmp = {
		.wrap =  std::string("bzero"),
		.library = std::string("libc-2.17.so"),
		.wrapperName = std::string("diogenes_bzero_wrapper"),
		.argMap = {std::make_pair(0,0),std::make_pair(1,1)},
	};
	DFW_MAP[tmp.wrap] = tmp;	

}

DyninstFunctionWraps::DyninstFunctionWraps(std::shared_ptr<DyninstProcess> proc) : _proc(proc) {}

bool DyninstFunctionWraps::InsertLoadStoreInstrimentation(BPatch_function * func, std::shared_ptr<BinaryLocationIDMap> bmap) {
	OneTimeFillMap();
	if (DFW_MAP.find(func->getName()) == DFW_MAP.end())
		return false;
	std::string tmpName = func->getName();
	BPatch_object * obj = func->getModule()->getObject();
	if (obj->pathName().find(DFW_MAP[tmpName].library) == std::string::npos)
		return false;

//	std::cerr << "[DyninstFunctionWraps::InsertLoadStoreInstrimentation] Inserting custom function wrap for function " 
//		      << func->getName() << " In library " <<  DFW_MAP[tmpName].library << std::endl;

	std::shared_ptr<DynOpsClass> ops = _proc->ReturnDynOps();
	std::vector<BPatch_function *> recordMemAccess = ops->FindFuncsByName(_proc->GetAddressSpace(), DFW_MAP[tmpName].wrapperName, NULL);
	std::set<BPatch_opCode> axs;
	axs.insert(BPatch_opLoad);
	axs.insert(BPatch_opStore);
	BPatchPointVecPtr loadsAndStores(func->findPoint(axs));
	BPatchPointVecPtr locationEntry(func->findPoint(BPatch_locEntry));
	assert(recordMemAccess.size() == 1);
	// assert(loadsAndStores->size() > 0);
	assert(locationEntry->size() > 0);

	std::string libname = obj->pathName();
	uint64_t addr = (uint64_t)(*locationEntry)[0]->getAddress();

	uint64_t libOffsetAddr = 0;
	if (!ops->GetFileOffset(_proc->GetAddressSpace(), (*locationEntry)[0], libOffsetAddr, true))
		libOffsetAddr = (uint64_t) (*locationEntry)[0]->getAddress();

	assert(bmap->AlreadyExists(libname, libOffsetAddr) == false);
	uint64_t id = bmap->StorePosition(libname, libOffsetAddr);

	std::vector<BPatch_snippet*> recordArgs;
	for (auto i : DFW_MAP[tmpName].argMap){
		assert(i.first <= recordArgs.size());
		while (i.first < recordArgs.size())
			recordArgs.push_back(new BPatch_constExpr(uint64_t(0)));
		recordArgs.push_back(new BPatch_paramExpr(i.second));
	}
	recordArgs.push_back(new BPatch_constExpr(id));

	std::cerr << "[DyninstFunctionWraps::InsertLoadStoreInstrimentation] Inserting custom function wrap for function " 
		      << func->getName() << " In library " <<  DFW_MAP[tmpName].library << " with id of " << id << std::endl;
	BPatch_funcCallExpr recordAddrCall(*(recordMemAccess[0]), recordArgs);
	assert(_proc->GetAddressSpace()->insertSnippet(recordAddrCall,*locationEntry) != NULL);
	return true;
}
