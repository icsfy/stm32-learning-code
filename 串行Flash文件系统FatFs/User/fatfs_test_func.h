#ifndef __FATFS_TEST_FUNC_H
#define __FATFS_TEST_FUNC_H

#include "fatfs/ff.h"
#include <stdio.h>
#include <string.h>

FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
);

void file_check(
  const TCHAR* path  /* [IN] Object name */
);

FRESULT get_space_info(
  const TCHAR* path,  /* [IN] Logical drive number */
  DWORD *fre_sect,
  DWORD *tot_sect
);

#endif /* __FATFS_TEST_FUNC_H */
